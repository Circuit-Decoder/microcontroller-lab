#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>   // NEW: library "WebSockets" by Markus Sattler

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const char* AP_NAME     = "DrawPad";
const char* AP_PASSWORD = "12345678";

WebServer server(80);               // sends the drawing page
WebSocketsServer webSocket(81);     // NEW: a live, always-open connection

int lastX = 0, lastY = 0;           // where the finger was last
bool needsUpdate = false;           // has the drawing changed?
unsigned long lastShow = 0;
bool inverted = false;

// ---------- The drawing page (HTML + JavaScript) ----------
const char PAGE[] = R"rawliteral(
<!DOCTYPE html>
<html><head>
<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
<title>Drawing Pad</title>
<style>
  body { font-family: sans-serif; margin: 12px; background: #eef3f8; text-align: center; }
  canvas { width: 100%; aspect-ratio: 2 / 1; background: black; touch-action: none;
           image-rendering: pixelated; border-radius: 8px; }
  button { font-size: 20px; padding: 10px 30px; margin-top: 12px; background: #1F3864;
           color: white; border: 0; border-radius: 8px; }
</style>
</head><body>
<h2>Draw on my ESP32</h2>
<canvas id="pad" width="128" height="64"></canvas>
<p id="status">Connecting...</p>
<button onclick="clearPad()">Clear</button>
<button id="eraser" onclick="toggleEraser()">Eraser: OFF</button>
<script>
  var pad = document.getElementById('pad');
  var pen = pad.getContext('2d');
  pen.fillStyle = '#7fdbff';
  var ws = new WebSocket('ws://' + location.hostname + ':81/');
  ws.onopen  = function () { document.getElementById('status').textContent = 'Connected - draw!'; };
  ws.onclose = function () { document.getElementById('status').textContent = 'Disconnected'; };
  var drawing = false, lastX = -1, lastY = -1;
  var erasing = false;                     // CHALLENGE 1
  function toggleEraser() {
    erasing = !erasing;
    document.getElementById('eraser').textContent = erasing ? 'Eraser: ON' : 'Eraser: OFF';
    pen.fillStyle = erasing ? 'black' : '#7fdbff';   // erase = draw in black
  }

  // finger position on the canvas -> pixel 0..127, 0..63
  function point(e) {
    var r = pad.getBoundingClientRect();
    var x = Math.floor((e.clientX - r.left) / r.width * 128);
    var y = Math.floor((e.clientY - r.top) / r.height * 64);
    return [Math.max(0, Math.min(127, x)), Math.max(0, Math.min(63, y))];
  }
  function send(cmd, x, y) { if (ws.readyState == 1) ws.send(cmd + ' ' + x + ' ' + y); }
  function line(x0, y0, x1, y1) {          // draw the same line on the phone
    var n = Math.max(Math.abs(x1 - x0), Math.abs(y1 - y0), 1);
    for (var i = 0; i <= n; i++) {
      pen.fillRect(Math.round(x0 + (x1 - x0) * i / n), Math.round(y0 + (y1 - y0) * i / n), 1, 1);
    }
  }
  pad.addEventListener('pointerdown', function (e) {
    drawing = true;
    var p = point(e); lastX = p[0]; lastY = p[1];
    line(lastX, lastY, lastX, lastY);
    send('D', lastX, lastY);               // D = pen down here
  });
  pad.addEventListener('pointermove', function (e) {
    if (!drawing) return;
    var p = point(e);
    if (p[0] == lastX && p[1] == lastY) return;   // same pixel: nothing to send
    line(lastX, lastY, p[0], p[1]);
    send(erasing ? 'E' : 'M', p[0], p[1]); // E = erase a line to here
    lastX = p[0]; lastY = p[1];
  });
  pad.addEventListener('pointerup', function () { drawing = false; });
  pad.addEventListener('pointerleave', function () { drawing = false; });
  function clearPad() {
    pen.clearRect(0, 0, 128, 64);
    send('C', 0, 0);                       // C = clear
  }
</script>
</body></html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }
  Wire.setClock(400000);              // talk to the OLED 4x faster
  display.setTextColor(SSD1306_WHITE);

  WiFi.softAP(AP_NAME, AP_PASSWORD);
  server.on("/", []() { server.send(200, "text/html", PAGE); });
  server.begin();
  webSocket.begin();
  webSocket.onEvent(onDrawingMessage);   // run this function for every message

  showWelcome();
}

void loop() {
  server.handleClient();
  webSocket.loop();                   // receive drawing messages

  // Update the OLED at most every 30 ms: fast, but not too often
  if (needsUpdate && millis() - lastShow > 30) {
    display.display();
    needsUpdate = false;
    lastShow = millis();
  }

  char key = keypad.getKey();
  if (key == '*') {                   // * = clear
    display.clearDisplay();
    needsUpdate = true;
  }
  else if (key == '#') {              // # = swap black and white
    inverted = !inverted;
    display.invertDisplay(inverted);
  }
}

// Called by the WebSocket library whenever the phone sends something
void onDrawingMessage(uint8_t client, WStype_t type, uint8_t* payload, size_t length) {
  if (type == WStype_CONNECTED) {         // a phone just connected
    display.clearDisplay();
    needsUpdate = true;
    return;
  }
  if (type != WStype_TEXT) return;

  char command = payload[0];              // 'D', 'M' or 'C'
  int x = 0, y = 0;
  sscanf((char*)payload + 1, "%d %d", &x, &y);   // read the two numbers

  if (command == 'D') {                   // pen down: one dot
    display.drawPixel(x, y, SSD1306_WHITE);
  }
  else if (command == 'M') {              // move: a line from the last point
    display.drawLine(lastX, lastY, x, y, SSD1306_WHITE);
  }
  else if (command == 'E') {              // CHALLENGE 1: erase = a BLACK line
    display.drawLine(lastX, lastY, x, y, SSD1306_BLACK);
  }
  else if (command == 'C') {              // clear
    display.clearDisplay();
  }
  lastX = x;
  lastY = y;
  needsUpdate = true;
}

void showWelcome() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Phone Drawing Pad");
  display.println("");
  display.print("WiFi: ");
  display.println(AP_NAME);
  display.print("Pass: ");
  display.println(AP_PASSWORD);
  display.println("");
  display.println("Open: 192.168.4.1");
  display.display();
}
