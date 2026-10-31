// Chapter 13, Practice 2 - Circle Area

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

// Waits for a number: digits, C = decimal point, D = delete, # = done.
double askNumber(String prompt) {
  Serial.print(prompt);
  String typed = "";
  while (true) {
    char key = keypad.getKey();
    if (key >= '0' && key <= '9') {
      typed += key;
      Serial.print(key);
    } else if (key == 'C' && typed.indexOf('.') == -1) {
      typed += '.';
      Serial.print('.');
    } else if (key == 'D' && typed.length() > 0) {
      typed.remove(typed.length() - 1);
      Serial.print(" [del] ");
    } else if (key == '#' && typed.length() > 0) {
      Serial.println();
      return typed.toDouble();
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  double r = askNumber("Radius (C = point): ");
  double area = PI * pow(r, 2);
  Serial.print("Area: ");
  Serial.println(area, 3);
  Serial.println();
}
