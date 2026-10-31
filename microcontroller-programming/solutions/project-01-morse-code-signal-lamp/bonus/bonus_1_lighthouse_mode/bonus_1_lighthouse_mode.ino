// Project 01, Bonus 1 - Lighthouse Mode
// The green lamp is on all the time and goes DARK while Button A is held.
// Button B still lights the red "please repeat" lamp.

const int BUTTON_A    = 15;
const int BUTTON_B    = 23;
const int GREEN_LED   = 18;   // the signal lamp
const int RED_LED     = 19;   // "please repeat"
const int BUILTIN_LED = 2;

void setup() {
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);
}

void loop() {
  // released reads HIGH (lamp on), pressed reads LOW (lamp dark)
  digitalWrite(GREEN_LED, digitalRead(BUTTON_A));
  digitalWrite(BUILTIN_LED, digitalRead(BUTTON_A));
  digitalWrite(RED_LED, !digitalRead(BUTTON_B));
}
