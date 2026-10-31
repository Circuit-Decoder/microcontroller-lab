// Chapter 7, Practice 1 - Tidy Project 03
// The PIN lock with its drawing moved into functions.

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

const int GREEN_LED  = 18;
const int RED_LED    = 19;
const int BUZZER_PIN = 4;

String correctPIN = "1234";
String enteredPIN = "";
bool lightOn = false;

void setup() {
  Serial.begin(115200);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
  }
  display.setTextColor(SSD1306_WHITE);
  showEntryScreen();
}

void loop() {
  char key = keypad.getKey();
  if (key == 0) return;

  if (key == '#') {
    if (enteredPIN == correctPIN) {
      lightOn = true;
      digitalWrite(GREEN_LED, HIGH);
      showMessage("Light ON");
      tone(BUZZER_PIN, 2000, 150);
      delay(1000);
    } else {
      showMessage("Wrong PIN");
      digitalWrite(RED_LED, HIGH);
      tone(BUZZER_PIN, 200, 600);
      delay(2000);
      digitalWrite(RED_LED, LOW);
    }
    enteredPIN = "";
  } else if (key == '*') {
    enteredPIN = "";
  } else if (key == 'D') {
    lightOn = false;
    digitalWrite(GREEN_LED, LOW);
  } else if (key >= '0' && key <= '9') {
    enteredPIN += key;
  }
  showEntryScreen();               // one call replaces the redraw flag
}

void showEntryScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Enter PIN:");
  if (lightOn) {
    display.println("Light: ON");
  } else {
    display.println("Light: OFF");
  }
  display.setCursor(0, 24);
  display.setTextSize(2);
  display.println(enteredPIN);
  display.setTextSize(1);
  display.setCursor(0, 56);
  display.print("#=OK *=Clear D=Off");
  display.display();
}

void showMessage(String text) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 16);
  display.println(text);
  display.display();
}
