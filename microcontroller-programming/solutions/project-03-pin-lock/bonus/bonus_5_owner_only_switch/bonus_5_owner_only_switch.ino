// Project 03, Bonus 5 - Owner-Only Switch
// D also asks for the PIN: type the PIN, then D to switch the light off.

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

const int GREEN_LED  = 18;   // the protected light
const int RED_LED    = 19;   // wrong-PIN warning
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
  drawEntry();
}

void loop() {
  char key = keypad.getKey();
  if (key == 0) return;

  if (key == '#' || key == 'D') {            // both need the right PIN
    if (enteredPIN == correctPIN) {
      lightOn = (key == '#');                // # = on, D = off
      digitalWrite(GREEN_LED, lightOn);
      message(lightOn ? "Light ON" : "Light OFF");
      tone(BUZZER_PIN, 2000, 150);
      delay(1000);
    } else {
      message("Wrong PIN");
      digitalWrite(RED_LED, HIGH);
      tone(BUZZER_PIN, 200, 600);
      delay(2000);
      digitalWrite(RED_LED, LOW);
    }
    enteredPIN = "";
  } else if (key == '*') {
    enteredPIN = "";
  } else if (key >= '0' && key <= '9') {
    enteredPIN += key;
  }
  drawEntry();
}

void drawEntry() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Enter PIN:");
  display.println(lightOn ? "Light: ON" : "Light: OFF");
  display.setTextSize(2);
  display.setCursor(0, 24);
  display.println(enteredPIN);
  display.setTextSize(1);
  display.setCursor(0, 56);
  display.print("#=On D=Off *=Clear");
  display.display();
}

void message(String text) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 16);
  display.println(text);
  display.display();
}
