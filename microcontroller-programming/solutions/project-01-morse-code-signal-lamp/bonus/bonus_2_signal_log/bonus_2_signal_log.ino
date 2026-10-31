// Project 01, Bonus 2 - Signal Log
// The lamp works as before, and Button A's reading goes to the
// Serial Monitor: 1 = released, 0 = pressed (lamp on).

const int BUTTON_A    = 15;
const int BUTTON_B    = 23;
const int GREEN_LED   = 18;
const int RED_LED     = 19;
const int BUILTIN_LED = 2;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);
}

void loop() {
  digitalWrite(GREEN_LED, !digitalRead(BUTTON_A));
  digitalWrite(BUILTIN_LED, !digitalRead(BUTTON_A));
  digitalWrite(RED_LED, !digitalRead(BUTTON_B));
  Serial.println(digitalRead(BUTTON_A));
  delay(20);              // 50 readings a second: short enough for Morse
}
