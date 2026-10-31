// Chapter 14, Practice 1 - Highest and Lowest
// E14.1, plus the highest and lowest mark found in one loop.

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

const int MAX_VALUES = 5;
int marks[MAX_VALUES];
int count = 0;                    // how many boxes are used so far
String typed = "";

void setup() {
  Serial.begin(115200);
  Serial.println("Type a mark, then #. Up to 5 marks.");
}

void loop() {
  char key = keypad.getKey();
  if (!key) return;

  if (key >= '0' && key <= '9') {
    typed += key;
  } else if (key == '#' && typed.length() > 0) {
    if (count < MAX_VALUES) {     // never go past the end
      marks[count] = typed.toInt();
      count++;
    } else {
      Serial.println("Full!");
    }
    typed = "";
    printAll();
  }
}

void printAll() {
  Serial.print(count);
  Serial.print(" stored: ");
  for (int i = 0; i < count; i++) {
    Serial.print(marks[i]);
    Serial.print(" ");
  }
  Serial.println();
  if (count == 0) return;

  int highest = marks[0];          // start with the first mark...
  int lowest = marks[0];
  for (int i = 1; i < count; i++) {  // ...and compare the rest
    if (marks[i] > highest) highest = marks[i];
    if (marks[i] < lowest) lowest = marks[i];
  }
  Serial.print("Highest: ");
  Serial.print(highest);
  Serial.print("  Lowest: ");
  Serial.println(lowest);
}
