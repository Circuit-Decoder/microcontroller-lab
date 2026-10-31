// Chapter 6, Practice 3 - Machine Beep
// E6.4 with a short beep every time the machine switches ON.

const int BUTTON_A   = 15;
const int BUTTON_B   = 23;
const int GREEN_LED  = 18;
const int RED_LED    = 19;
const int BUZZER_PIN = 4;

bool wasOn = false;    // was the machine on in the previous loop?

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
}

void loop() {
  bool aPressed = (digitalRead(BUTTON_A) == LOW);
  bool bPressed = (digitalRead(BUTTON_B) == LOW);
  bool isOn = aPressed && bPressed;

  if (isOn && !wasOn) {                 // it has JUST switched on
    tone(BUZZER_PIN, 1500, 100);
  }

  if (isOn) {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  } else if (aPressed || bPressed) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  } else {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
  }

  wasOn = isOn;
  delay(20);
}
