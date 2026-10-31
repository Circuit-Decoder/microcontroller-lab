// Chapter 8, Practice 2 - Prices on Screen
// E8.4 with the answer on the OLED: item in yellow, price big.

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

const int SIZE = 5;
int prices[SIZE] = {10, 25, 15, 40, 30};   // items 0 to 4
String typed = "";

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
  }
  display.setTextColor(SSD1306_WHITE);
  show("Item 0-4, then #", "");
}

void loop() {
  char key = keypad.getKey();
  if (!key) return;

  if (key >= '0' && key <= '9') {
    typed += key;
    Serial.print("Typed so far: ");
    Serial.println(typed);
  } else if (key == '#') {
    int item = typed.toInt();        // "3" becomes 3
    if (item >= 0 && item < SIZE) {
      show("Item " + String(item), String(prices[item]) + " Tk");
    } else {
      show("Item " + String(item), "None!");
    }
    typed = "";                      // ready for the next number
  }
}

// title in the yellow strip, big text in the blue area
void show(String title, String big) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(title);
  display.setTextSize(3);
  display.setCursor(0, 24);
  display.println(big);
  display.display();
}
