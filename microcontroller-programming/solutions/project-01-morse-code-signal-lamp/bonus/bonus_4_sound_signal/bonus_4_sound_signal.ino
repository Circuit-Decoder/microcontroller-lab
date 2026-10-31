// Project 01, Bonus 4 - Sound Signal (after Chapter 6)
// The buzzer sounds while Button A is held, together with the lamp.

const int BUTTON_A    = 15;
const int BUTTON_B    = 23;
const int GREEN_LED   = 18;
const int RED_LED     = 19;
const int BUILTIN_LED = 2;
const int BUZZER      = 4;    // passive buzzer on GPIO 4

void setup() {
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);
}

void loop() {
  bool aPressed = (digitalRead(BUTTON_A) == LOW);
  digitalWrite(GREEN_LED, aPressed);
  digitalWrite(BUILTIN_LED, aPressed);
  digitalWrite(RED_LED, digitalRead(BUTTON_B) == LOW);

  if (aPressed) {
    tone(BUZZER, 800);        // a steady 800 Hz beep, like a radio operator
  } else {
    noTone(BUZZER);
  }
}
