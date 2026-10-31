// Chapter 11, Practice 3 - Percentage
// Type a mark out of 75, press #, see it as a percentage.

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

String typed = "";

void setup() {
  Serial.begin(115200);
  Serial.println("Type a mark out of 75, then #");
}

void loop() {
  char key = keypad.getKey();
  if (!key) return;

  if (key >= '0' && key <= '9') {
    typed += key;
  } else if (key == '#') {
    int mark = typed.toInt();
    if (mark > 75) {
      Serial.println("Out of 75, please");
    } else {
      double percent = mark * 100.0 / 75;   // 100.0: decimal maths
      Serial.print(mark);
      Serial.print("/75 = ");
      Serial.print(percent, 1);            // 1 decimal place
      Serial.println("%");
    }
    typed = "";
  }
}
