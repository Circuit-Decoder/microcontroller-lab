// Chapter 6, Practice 1 - Goal of Ten
// E6.2 with a goal of 10 presses; the green LED lights at press 5.

const int BUTTON_PIN = 15;
const int GREEN_LED  = 18;
const int RED_LED    = 19;
const int GOAL       = 10;
const int HALFWAY    = 5;

int count = 0;
int lastState = HIGH;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  Serial.println("Press the button 10 times.");
}

void loop() {
  int state = digitalRead(BUTTON_PIN);
  if (state == LOW && lastState == HIGH) {   // a new press
    count = count + 1;
    Serial.print("Presses: ");
    Serial.println(count);
    if (count >= HALFWAY) {
      digitalWrite(GREEN_LED, HIGH);         // halfway sign
    }
    if (count >= GOAL) {
      digitalWrite(RED_LED, HIGH);
      Serial.println("Goal reached!");
    }
  }
  lastState = state;
  delay(20);
}
