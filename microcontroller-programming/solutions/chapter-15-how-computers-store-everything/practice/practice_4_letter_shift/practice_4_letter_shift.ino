// Chapter 15, Practice 4 - Letter Shift
// Each key and the character after it in the code table.

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

void setup() {
  Serial.begin(115200);
  Serial.println("Press any key...");
}

void loop() {
  char key = keypad.getKey();
  if (!key) return;
  char next = key + 1;               // one more in the code table
  Serial.print(key);
  Serial.print(" -> ");
  Serial.print(next);
  Serial.print("   (codes ");
  Serial.print((int)key);
  Serial.print(" -> ");
  Serial.print((int)next);
  Serial.println(")");
}
