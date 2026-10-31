// Chapter 2, Practice 1 - Mirror Lamp
// E2.4: Button A lights the green LED AND the built-in LED together.

const int BUTTON_A    = 15;   // Button A: GPIO 15 to GND
const int GREEN_LED   = 18;   // green LED + 220 ohm on GPIO 18
const int BUILTIN_LED = 2;    // the small LED on the board

void setup() {
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);
}

void loop() {
  digitalWrite(GREEN_LED, !digitalRead(BUTTON_A));
  digitalWrite(BUILTIN_LED, !digitalRead(BUTTON_A));
}
