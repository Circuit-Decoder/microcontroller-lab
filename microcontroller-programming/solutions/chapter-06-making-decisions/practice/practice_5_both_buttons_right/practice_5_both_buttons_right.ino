// Chapter 6, Practice 5 - Both Buttons Right
// E6.6: the "right" sound plays only when BOTH buttons are pressed.

const int BUTTON_A   = 15;
const int BUTTON_B   = 23;
const int BUZZER_PIN = 4;

void setup() {
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  bool aPressed = (digitalRead(BUTTON_A) == LOW);
  bool bPressed = (digitalRead(BUTTON_B) == LOW);

  if (aPressed && bPressed) {              // "right": both together
    tone(BUZZER_PIN, 1500, 100);
    delay(150);
    tone(BUZZER_PIN, 2000, 100);
    delay(400);
  } else if (bPressed) {                   // "wrong": B alone
    tone(BUZZER_PIN, 200, 600);
    delay(800);
  }
}
