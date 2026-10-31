// Project 02, Bonus 2 - Typing Speed (after Chapter 18)
// Press A to start; count every key for 10 seconds, then show the result.

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

const unsigned long TEST_TIME = 10000;   // 10 seconds
bool running = false;
unsigned long startTime = 0;
int presses = 0;

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
  }
  display.setTextColor(SSD1306_WHITE);
  showText("Press A to start", "");
}

void loop() {
  char key = keypad.getKey();

  if (!running && key == 'A') {           // start the test
    running = true;
    presses = 0;
    startTime = millis();
    showText("GO! Press keys", "0");
  } else if (running && key) {            // count every key
    presses++;
    showText("GO! Press keys", String(presses));
  }

  if (running && millis() - startTime >= TEST_TIME) {   // time is up
    running = false;
    String result = String(presses * 6) + "/min";
    showText("Time! A = again", result);
    Serial.print("Presses in 10 s: ");
    Serial.println(presses);
  }
}

void showText(String title, String big) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(title);
  display.setTextSize(3);
  display.setCursor(0, 28);
  display.println(big);
  display.display();
}
