// Chapter 2, Practice 2 - Night Light
// E2.5, but the red LED shines when Button B is NOT pressed,
// and goes dark while you hold it.

const int BUTTON_A  = 15;
const int BUTTON_B  = 23;
const int GREEN_LED = 18;
const int RED_LED   = 19;

void setup() {
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
}

void loop() {
  digitalWrite(GREEN_LED, !digitalRead(BUTTON_A));
  // no ! here: released reads 1 (HIGH, LED on), pressed reads 0 (LED off)
  digitalWrite(RED_LED, digitalRead(BUTTON_B));
}
