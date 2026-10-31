// Chapter 8, Practice 3 - Month Names
// Type 1-12 and press # to see the month's name.

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

String months[12] = {
  "January", "February", "March", "April", "May", "June",
  "July", "August", "September", "October", "November", "December"
};
String typed = "";

void setup() {
  Serial.begin(115200);
  Serial.println("Type a month number (1-12), then #");
}

void loop() {
  char key = keypad.getKey();
  if (!key) return;

  if (key >= '0' && key <= '9') {
    typed += key;
  } else if (key == '#') {
    int month = typed.toInt();
    if (month >= 1 && month <= 12) {
      Serial.println(months[month - 1]);   // month 1 is index 0
    } else {
      Serial.println("No such month");
    }
    typed = "";
  }
}
