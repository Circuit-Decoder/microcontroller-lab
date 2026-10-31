// Chapter 6, Practice 4 - Fresh Start
// E6.5, but typed is cleared after every decision.

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>

const int SCREEN_WIDTH  = 128;
const int SCREEN_HEIGHT = 64;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

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

const int GREEN_LED = 18;
const int RED_LED   = 19;

String secret = "42";
String typed  = "";

void setup() {
  Serial.begin(115200);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
  }
  display.setTextColor(SSD1306_WHITE);
  showTyped();
}

void loop() {
  char key = keypad.getKey();
  if (key == 0) return;

  if (key == '#') {
    if (typed == secret) {
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED, LOW);
      Serial.println("Match!");
    } else {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, HIGH);
      Serial.println("No match.");
    }
    typed = "";                    // NEW: the next try starts fresh
  } else if (key == '*') {
    typed = "";
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
  } else {
    typed += key;
  }
  showTyped();
}

void showTyped() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Code word, then #");
  display.setCursor(0, 16);
  display.setTextSize(2);
  display.println(typed);
  display.display();
}
