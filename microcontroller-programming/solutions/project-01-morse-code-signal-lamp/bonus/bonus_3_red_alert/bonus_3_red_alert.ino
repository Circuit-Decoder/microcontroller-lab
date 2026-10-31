// Project 01, Bonus 3 - Red Alert
// Button B lights the red lamp AND the built-in LED.
// The built-in LED also follows Button A, as before.

const int BUTTON_A    = 15;
const int BUTTON_B    = 23;
const int GREEN_LED   = 18;
const int RED_LED     = 19;
const int BUILTIN_LED = 2;

void setup() {
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);
}

void loop() {
  digitalWrite(GREEN_LED, !digitalRead(BUTTON_A));
  digitalWrite(RED_LED, !digitalRead(BUTTON_B));
  digitalWrite(BUILTIN_LED, !digitalRead(BUTTON_A));   // follows A ...
  digitalWrite(BUILTIN_LED, !digitalRead(BUTTON_B));   // ... and then B
}
