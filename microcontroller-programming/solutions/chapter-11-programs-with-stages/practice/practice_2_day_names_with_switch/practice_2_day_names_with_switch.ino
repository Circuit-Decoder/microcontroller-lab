// Chapter 11, Practice 2 - Day Names with switch
// E8.2 again, with switch instead of an array.

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
  Serial.println("Press 0 to 6...");
}

void loop() {
  char key = keypad.getKey();
  if (!key) return;

  switch (key) {
    case '0': Serial.println("Saturday");  break;
    case '1': Serial.println("Sunday");    break;
    case '2': Serial.println("Monday");    break;
    case '3': Serial.println("Tuesday");   break;
    case '4': Serial.println("Wednesday"); break;
    case '5': Serial.println("Thursday");  break;
    case '6': Serial.println("Friday");    break;
    default:  Serial.println("Not a day key");
  }
}
