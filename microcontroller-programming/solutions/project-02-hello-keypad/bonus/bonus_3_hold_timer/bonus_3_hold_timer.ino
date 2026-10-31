// Project 02, Bonus 3 - Hold Timer (after Chapter 6)
// Shows how long each key was held down, in milliseconds.

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int SCREEN_WIDTH  = 128;
const int SCREEN_HEIGHT = 64;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

char heldKey = 0;
unsigned long pressTime = 0;

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
  }
  display.setTextColor(SSD1306_WHITE);
  keypad.addEventListener(keyEvent);   // the library calls keyEvent()
  show("Press and hold", "a key");
}

void loop() {
  keypad.getKey();                      // keeps the keypad scanning
}

// The library calls this when a key is pressed, held or released
void keyEvent(KeypadEvent key) {
  KeyState state = keypad.getState();
  if (state == PRESSED) {
    heldKey = key;
    pressTime = millis();
    show(String("Key ") + key, "held...");
  } else if (state == RELEASED && key == heldKey) {
    unsigned long held = millis() - pressTime;
    show(String("Key ") + key, String(held) + " ms");
    Serial.print(key);
    Serial.print(" held for ");
    Serial.print(held);
    Serial.println(" ms");
  }
}

void show(String title, String big) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(title);
  display.setTextSize(2);
  display.setCursor(0, 28);
  display.println(big);
  display.display();
}
