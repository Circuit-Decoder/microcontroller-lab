// Project 02, Bonus 4 - Faulty-Key Finder (after Chapter 8)
// Counts every key separately. Press each key exactly once;
// a key that types twice stands out with a count of 2 or more.

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

String labels = "123A456B789C*0#D";   // the 16 keys, row by row
int counts[16];                        // one counter per key

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
  }
  display.setTextColor(SSD1306_WHITE);
  for (int i = 0; i < 16; i++) counts[i] = 0;
  drawTable();
}

void loop() {
  char key = keypad.waitForKey();
  int index = labels.indexOf(key);     // which box belongs to this key
  if (index >= 0) counts[index]++;
  drawTable();
}

void drawTable() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Press each key once");
  for (int i = 0; i < 16; i++) {
    int row = i / 4;
    int col = i % 4;
    display.setCursor(col * 32, 16 + row * 12);
    display.print(labels[i]);
    display.print(':');
    display.print(counts[i]);
    if (counts[i] > 1) display.print('!');   // suspicious key
  }
  display.display();
}
