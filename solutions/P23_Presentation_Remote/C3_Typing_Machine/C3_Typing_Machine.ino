#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include "BluetoothKeys.h"      // our Bluetooth keyboard tab (Project 23)
#include "MultiTap.h"           // our phone keyboard (Project 13)

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

// The name the computer will see in its Bluetooth list
const char* REMOTE_NAME = "CD Keyboard";
bool wasConnected = false;

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }
  display.setTextColor(SSD1306_WHITE);
  mtMaxLength = 60;
  btBegin(REMOTE_NAME);
  showScreen();
}

void loop() {
  bool connected = btConnected();
  if (connected != wasConnected) {
    wasConnected = connected;
    showScreen();
  }
  char key = keypad.getKey();
  if (mtUpdate(key)) showScreen();           // 0-9 type letters, * deletes

  if (key == '#') {                          // # = send the words
    mtLockIn();
    sendText(mtText);
    mtClear();
    showScreen();
  }
  else if (key == 'A') {                     // A = send the words + Enter
    mtLockIn();
    sendText(mtText);
    btPress(KEY_ENTER);
    mtClear();
    showScreen();
  }
  else if (key == 'B') {                     // B = Backspace on the laptop
    btPress(0x2A);
  }
  else if (key == 'D') {                     // D = clear without sending
    mtClear();
    showScreen();
  }
}

// ---------- Turn one letter into a key press ----------
void sendChar(char c) {
  if (c >= 'A' && c <= 'Z') btPress(KEY_A_LETTER + (c - 'A'), MOD_SHIFT);  // capital letter
  else if (c >= 'a' && c <= 'z') btPress(KEY_A_LETTER + (c - 'a'));
  else if (c >= '0' && c <= '9') btDigit(c);
  else if (c == ' ') btPress(0x2C);          // space bar
  else if (c == '.') btPress(0x37);
  else if (c == ',') btPress(0x36);
  else if (c == '?') btPress(0x38, MOD_SHIFT);   // Shift + /
  else if (c == '!') btPress(0x1E, MOD_SHIFT);   // Shift + 1
}

void sendText(String text) {
  if (!btConnected()) return;
  for (int i = 0; i < text.length(); i++) {
    sendChar(text[i]);
  }
}

// ---------- Screen ----------
void showScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("BT KEYBOARD");
  display.setCursor(104, 0);
  display.print(btConnected() ? "BT" : "--");
  display.setCursor(0, 16);
  if (!btConnected()) {
    display.println("Pair the laptop with:");
    display.println(REMOTE_NAME);
  } else {
    display.print(mtPreview());
    display.println("_");
  }
  display.setCursor(0, 48);
  display.print("#=Send A=Send+Enter");
  display.setCursor(0, 56);
  display.print("*=Del B=Bksp D=Clear");
  display.display();
}
