// Chapter 5, Practice 3 - Key and Count
// E5.3 plus a count of every key press in the yellow strip.

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

int presses = 0;   // global: it keeps its value between loops

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
  }
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Press a");
  display.println("key!");
  display.display();
}

void loop() {
  char key = keypad.waitForKey();
  presses = presses + 1;

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("You pressed:");
  display.print("Presses: ");        // second line of the yellow strip
  display.println(presses);
  display.setTextSize(4);
  display.setCursor(0, 24);
  display.println(key);
  display.display();
}
