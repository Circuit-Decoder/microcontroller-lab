// Project 02, Bonus 1 - Key History
// The last three keys, newest on the right, like 7 3 9.

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

char oldest = ' ';    // three boxes: oldest, middle, newest
char middle = ' ';
char newest = ' ';

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
  }
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  char key = keypad.waitForKey();

  // hand over in the right order, oldest first (like before = now)
  oldest = middle;
  middle = newest;
  newest = key;

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Last three keys");
  display.setTextSize(3);
  display.setCursor(10, 28);
  display.print(oldest);
  display.print(' ');
  display.print(middle);
  display.print(' ');
  display.print(newest);
  display.display();
}
