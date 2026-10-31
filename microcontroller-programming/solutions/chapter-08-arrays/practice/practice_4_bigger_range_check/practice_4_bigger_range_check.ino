// Chapter 8, Practice 4 - Bigger Range Check
// Only SIZE and the list changed: items 0-7 now exist.

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

const int SIZE = 8;
int prices[SIZE] = {10, 25, 15, 40, 30, 55, 20, 35};   // items 0-7
String typed = "";

void setup() {
  Serial.begin(115200);
  Serial.println("Type an item number, then #");
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
      Serial.print("Item ");
      Serial.print(item);
      Serial.print(" costs ");
      Serial.print(prices[item]);
      Serial.println(" taka");
    } else {
      Serial.println("No such item");
    }
    typed = "";                      // ready for the next number
  }
}
