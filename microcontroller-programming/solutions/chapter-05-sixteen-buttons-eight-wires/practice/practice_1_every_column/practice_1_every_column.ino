// Chapter 5, Practice 1 - Every Column
// Scan columns 1 and 2 by hand and print both results on one line.

const int ROW_1 = 13;     // keypad pin 1
const int ROW_2 = 12;     // keypad pin 2
const int ROW_3 = 14;     // keypad pin 3
const int ROW_4 = 27;     // keypad pin 4
const int COLUMN_1 = 26;  // keypad pin 5
const int COLUMN_2 = 25;  // keypad pin 6

void setup() {
  Serial.begin(115200);
  pinMode(ROW_1, INPUT_PULLUP);
  pinMode(ROW_2, INPUT_PULLUP);
  pinMode(ROW_3, INPUT_PULLUP);
  pinMode(ROW_4, INPUT_PULLUP);
  pinMode(COLUMN_1, INPUT);       // both columns "off" to start with
  pinMode(COLUMN_2, INPUT);
}

void loop() {
  // column 1: make it GND, read the rows, then switch it off again
  pinMode(COLUMN_1, OUTPUT);
  digitalWrite(COLUMN_1, LOW);
  Serial.print("col 1: ");
  Serial.print(digitalRead(ROW_1));
  Serial.print(digitalRead(ROW_2));
  Serial.print(digitalRead(ROW_3));
  Serial.print(digitalRead(ROW_4));
  pinMode(COLUMN_1, INPUT);       // back to listening before column 2

  // column 2: the same
  pinMode(COLUMN_2, OUTPUT);
  digitalWrite(COLUMN_2, LOW);
  Serial.print("   col 2: ");
  Serial.print(digitalRead(ROW_1));
  Serial.print(digitalRead(ROW_2));
  Serial.print(digitalRead(ROW_3));
  Serial.println(digitalRead(ROW_4));
  pinMode(COLUMN_2, INPUT);

  delay(200);
}
