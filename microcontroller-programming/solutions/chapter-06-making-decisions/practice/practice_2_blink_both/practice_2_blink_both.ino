// Chapter 6, Practice 2 - Blink Both
// E6.3 plus key 1: both LEDs blink once.

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

void setup() {
  Serial.begin(115200);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
  }
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Pick a light:");
  display.setCursor(0, 16);
  display.println("A green   B red");
  display.println("C both    D off");
  display.println("1 blink");
  display.display();
}

void loop() {
  char key = keypad.getKey();
  if (key == 0) return;

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("You pressed ");
  display.println(key);
  display.setCursor(0, 16);
  display.setTextSize(2);

  if (key == 'A') {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    display.println("Green");
  } else if (key == 'B') {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    display.println("Red");
  } else if (key == 'C') {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, HIGH);
    display.println("Both");
  } else if (key == 'D') {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
    display.println("Off");
  } else if (key == '1') {
    display.println("Blink!");
    display.display();
    digitalWrite(GREEN_LED, HIGH);       // both on...
    digitalWrite(RED_LED, HIGH);
    delay(300);
    digitalWrite(GREEN_LED, LOW);        // ...and both off
    digitalWrite(RED_LED, LOW);
  } else {
    display.println("Not a");
    display.println("choice");
  }
  display.display();
}
