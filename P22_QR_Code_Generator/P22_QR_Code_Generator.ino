#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <qrcode.h>             // NEW: QR codes - built into the ESP32, no library needed
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

// CHANGE THESE: B makes a QR code that joins this Wi-Fi network
const char* WIFI_NAME     = "CircuitDecoder";
const char* WIFI_PASSWORD = "12345678";

const int MAX_VERSION = 3;      // version 3 = 29 x 29 squares (fits the screen)
const int SCALE = 2;            // each square = 2 x 2 pixels

bool showingCode = false;
String qrLabel = "";            // shown next to the code
int qrLength = 0;

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }
  display.setTextColor(SSD1306_WHITE);
  mtMaxLength = 50;
  showTyping();
}

void loop() {
  char key = keypad.getKey();

  if (showingCode) {                    // any key: back to typing
    if (key) {
      showingCode = false;
      setBrightness(207);               // normal brightness again
      showTyping();
    }
    return;
  }

  if (mtUpdate(key)) showTyping();

  if (key == '#') {                     // # = make the QR code
    mtLockIn();
    if (mtText.length() > 0) {
      showQR(mtText, "Text QR");
    }
  }
  else if (key == 'B') {                // B = Wi-Fi QR code
    String wifi = "WIFI:T:WPA;S:" + String(WIFI_NAME) + ";P:" + String(WIFI_PASSWORD) + ";;";
    showQR(wifi, "WiFi QR");
  }
  else if (key == 'D') {                // D = clear the text
    mtClear();
    showTyping();
  }
}

// ---------- Make a QR code ----------
void showQR(String text, String label) {
  qrLabel = label;
  qrLength = text.length();

  esp_qrcode_config_t config;
  config.display_func = drawQR;                 // callback: run drawQR() when ready
  config.max_qrcode_version = MAX_VERSION;      // no bigger than version 3
  config.qrcode_ecc_level = ESP_QRCODE_ECC_LOW;

  if (esp_qrcode_generate(&config, text.c_str()) != ESP_OK) {
    display.clearDisplay();                     // too long for the screen
    display.setCursor(0, 20);
    display.println("Text too long!");
    display.println("Try fewer letters.");
    display.display();
  }
  showingCode = true;
}

// ---------- Draw it: called by esp_qrcode_generate() ----------
void drawQR(esp_qrcode_handle_t qr) {
  int squares = esp_qrcode_get_size(qr);        // 21, 25 or 29
  int version = (squares - 17) / 4;             // 21 -> 1, 25 -> 2, 29 -> 3

  display.clearDisplay();
  setBrightness(40);                            // dimmer: less glow, sharper squares
  int size = squares * SCALE;                   // 42, 50 or 58 pixels
  int top = 0;
  if (size <= 44) top = 16;                     // a small code fits below the yellow strip
  // A white square behind the code: phones like dark squares on white
  display.fillRect(0, top, 64, 64 - top, SSD1306_WHITE);
  int startX = (64 - size) / 2;                 // centre it in the white square
  int startY = top + (64 - top - size) / 2;

  for (int y = 0; y < squares; y++) {
    for (int x = 0; x < squares; x++) {
      if (esp_qrcode_get_module(qr, x, y)) {    // a dark square?
        display.fillRect(startX + x * SCALE, startY + y * SCALE, SCALE, SCALE, SSD1306_BLACK);
      }
    }
  }

  // Information on the right (10 letters fit)
  display.setTextSize(1);
  display.setCursor(70, 0);
  display.print("Scan me!");
  display.setCursor(70, 18);
  display.print(qrLabel);
  display.setCursor(70, 28);
  display.print(qrLength);
  display.print(" chars");
  display.setCursor(70, 46);
  display.print("Version ");
  display.print(version);
  display.setCursor(70, 56);
  display.print(squares);
  display.print(" x ");
  display.print(squares);
  display.display();
}

// OLED brightness: 207 = normal, 1 = dimmest
void setBrightness(byte level) {
  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(level);
}

void showTyping() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("QR CODE MAKER");
  display.println("");
  display.print(mtPreview());
  display.println("_");
  display.setCursor(0, 48);
  display.print("#=Make QR  B=WiFi QR");
  display.setCursor(0, 56);
  display.print("*=Del      D=Clear");
  display.display();
}
