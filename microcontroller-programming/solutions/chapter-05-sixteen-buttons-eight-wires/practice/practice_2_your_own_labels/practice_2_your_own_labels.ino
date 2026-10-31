// Chapter 5, Practice 2 - Your Own Labels
// The keymap is just a table: A-D now type the maths symbols + - * /.

#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', '+'},     // was A
  {'4', '5', '6', '-'},     // was B
  {'7', '8', '9', '*'},     // was C
  {'*', '0', '#', '/'}      // was D
};
byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(115200);
  Serial.println("Press a key...");
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    Serial.print("Key: ");
    Serial.println(key);
  }
}
