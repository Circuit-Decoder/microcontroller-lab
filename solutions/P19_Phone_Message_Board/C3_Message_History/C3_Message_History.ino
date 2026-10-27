#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <WiFi.h>
#include <WebServer.h>          // NEW: turns the ESP32 into a web server
#include <DNSServer.h>          // NEW: sends every web address to the ESP32
#include "MultiTap.h"           // our keyboard from Project 13

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

// ---------- The ESP32's own Wi-Fi network ----------
const char* AP_NAME     = "CircuitDecoder";
const char* AP_PASSWORD = "12345678";      // at least 8 characters

WebServer server(80);           // port 80 = normal web pages
DNSServer dns;                  // answers "where is ...?" with our address
int phonesJoined = 0;           // how many phones are on our Wi-Fi

String lastMessage = "";        // the last message from the phone
// CHALLENGE 3: the last 3 messages. history[0] is the newest.
String history[3] = {"", "", ""};
int viewing = 0;                // which one is on the screen (0, 1 or 2)
int messageCount = 0;
String reply = "";              // the reply typed on the keypad
bool typingReply = false;

// ---------- The web page the phone will see (HTML) ----------
const char PAGE[] = R"rawliteral(
<!DOCTYPE html>
<html><head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Message Board</title>
<style>
  body { font-family: sans-serif; margin: 20px; background: #eef3f8; }
  input, button { width: 100%; font-size: 20px; padding: 10px; box-sizing: border-box; }
  button { margin-top: 10px; background: #1F3864; color: white; border: 0; border-radius: 8px; }
  #box { margin-top: 20px; padding: 12px; background: white; border-radius: 8px; }
</style>
</head><body>
<h2>Send to my ESP32</h2>
<form action="/send">
  <input name="msg" maxlength="100" placeholder="Type a message">
  <button>Send</button>
</form>
<div id="box">Reply from ESP32: <b id="reply">...</b></div>
<script>
  setInterval(function () {
    fetch('/reply').then(r => r.text()).then(t => {
      document.getElementById('reply').textContent = t;
    });
  }, 2000);
</script>
</body></html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }
  display.setTextColor(SSD1306_WHITE);
  mtMaxLength = 40;

  WiFi.softAP(AP_NAME, AP_PASSWORD);        // start our own Wi-Fi network
  dns.start(53, "*", WiFi.softAPIP());      // any address -> the ESP32

  server.on("/", handleRoot);               // address  ->  function
  server.on("/send", handleSend);
  server.on("/reply", handleReply);
  server.onNotFound(handleNotFound);        // any other address -> main page
  server.begin();

  showScreen();
}

void loop() {
  dns.processNextRequest();                 // answer address questions
  server.handleClient();                    // answer the phone, if it asks

  int n = WiFi.softAPgetStationNum();       // did a phone join or leave?
  if (n != phonesJoined) {
    phonesJoined = n;
    showScreen();
  }

  char key = keypad.getKey();
  if (typingReply) {
    if (mtUpdate(key)) showScreen();
    if (key == '#') {                       // # = send the reply
      mtLockIn();
      reply = mtText;
      typingReply = false;
      showScreen();
    }
    else if (key == 'D') {                  // D = cancel
      typingReply = false;
      showScreen();
    }
  }
  else if (key == 'B' && messageCount > 0) {   // CHALLENGE 3: B = older message
    viewing = (viewing + 1) % 3;
    if (history[viewing] == "") viewing = 0;   // no older one: back to newest
    showScreen();
  }
  else if (key == 'A') {                    // A = write a reply
    mtClear();
    typingReply = true;
    showScreen();
  }
}

// ---------- Web server handlers ----------
void handleRoot() {
  server.send(200, "text/html", PAGE);      // 200 = "OK, here it is"
}

void handleSend() {
  lastMessage = server.arg("msg");          // the text from the form
  history[2] = history[1];                  // move the older ones down
  history[1] = history[0];                  // (from the bottom up, like Project 17)
  history[0] = lastMessage;
  viewing = 0;
  messageCount++;
  showScreen();
  server.sendHeader("Location", "/");       // go back to the main page
  server.send(303);
}

// Phones check "am I online?" by opening special addresses.
// We send them to our page, so the phone opens it by itself.
void handleNotFound() {
  server.sendHeader("Location", "http://" + WiFi.softAPIP().toString() + "/");
  server.send(302, "text/plain", "");
}

void handleReply() {
  server.send(200, "text/plain", reply);
}

// ---------- Screen ----------
void showScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);

  if (typingReply) {
    display.println("Your reply:");
    display.print(mtPreview());
    display.println("_");
    display.setCursor(0, 56);
    display.print("#=Send *=Del D=Back");
  }
  else if (messageCount == 0) {             // nothing yet: show how to connect
    display.println("On your phone, join:");
    display.print("WiFi: ");
    display.println(AP_NAME);
    display.print("Pass: ");
    display.println(AP_PASSWORD);
    display.println("");
    display.println("Then open in browser:");
    display.println(WiFi.softAPIP().toString());   // 192.168.4.1
    display.setCursor(0, 56);
    display.print("Phones joined: ");
    display.print(phonesJoined);
  }
  else {
    display.print("Message ");
    display.print(messageCount - viewing);
    display.println(":");
    display.println(history[viewing]);
    display.setCursor(0, 56);
    display.print("A=Reply  B=Older");
  }
  display.display();
}
