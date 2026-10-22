#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <qrcode.h>             // NEW: QR codes - built into the ESP32, no library needed
#include "MultiTap.h"           // our keyboard from Project 13
#include <EEPROM.h>             // CHALLENGE 3: read the Contact Book from Project 15

// The same memory layout as Project 15
#define NAME_SLOT_SIZE 10
#define NUMBER_SLOT_SIZE 15
#define CONTACT_SLOT_SIZE (NAME_SLOT_SIZE + NUMBER_SLOT_SIZE)
#define MAX_CONTACTS 5
#define EEPROM_SIZE (CONTACT_SLOT_SIZE * MAX_CONTACTS)
int contactSlot = -1;           // the contact shown last

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
  EEPROM.begin(EEPROM_SIZE);
  showTyping();
}

void loop() {
  char key = keypad.getKey();

  if (showingCode) {                    // any key: back to typing
    if (key) {
      showingCode = false;
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
  else if (key == 'A') {                // CHALLENGE 3: A = next saved contact
    showNextContact();
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
  // A white square behind the code: phones like dark squares on white
  display.fillRect(0, 0, 64, 64, SSD1306_WHITE);
  int start = (64 - squares * SCALE) / 2;       // centre it in the white square

  for (int y = 0; y < squares; y++) {
    for (int x = 0; x < squares; x++) {
      if (esp_qrcode_get_module(qr, x, y)) {    // a dark square?
        display.fillRect(start + x * SCALE, start + y * SCALE, SCALE, SCALE, SSD1306_BLACK);
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

// ---------- CHALLENGE 3: the Contact Book ----------
String readText(int start, int size) {
  String s = "";
  for (int i = 0; i < size; i++) {
    char c = EEPROM.read(start + i);
    if (c == '\0') break;
    s += c;
  }
  return s;
}

// Same test as Project 15: a used slot has a number made only of digits
bool numberIsGood(String n) {
  if (n.length() == 0) return false;
  for (int i = 0; i < n.length(); i++) {
    if (n[i] < '0' || n[i] > '9') return false;
  }
  return true;
}

void showNextContact() {
  for (int tries = 0; tries < MAX_CONTACTS; tries++) {
    contactSlot = (contactSlot + 1) % MAX_CONTACTS;
    int start = contactSlot * CONTACT_SLOT_SIZE;
    String number = readText(start + NAME_SLOT_SIZE, NUMBER_SLOT_SIZE);
    if (numberIsGood(number)) {
      String name = readText(start, NAME_SLOT_SIZE);
      showQR("tel:" + number, name);       // the name is the label
      return;
    }
  }
  display.clearDisplay();                 // nothing saved yet
  display.setCursor(0, 20);
  display.println("No contacts saved.");
  display.println("Run Project 15 first.");
  display.display();
  showingCode = true;                     // any key = back
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
  display.print("A=Contact *=Del D=Clr");
  display.display();
}
