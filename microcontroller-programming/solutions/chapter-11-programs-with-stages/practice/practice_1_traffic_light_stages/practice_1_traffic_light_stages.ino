// Chapter 11, Practice 1 - Traffic Light Stages
// green -> yellow (both LEDs) -> red, every 2 seconds.

const int GREEN_LED = 18;
const int RED_LED   = 19;

int stage = 1;     // 1 = green, 2 = yellow, 3 = red

void setup() {
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
}

void loop() {
  switch (stage) {
    case 1:
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED, LOW);
      stage = 2;
      break;
    case 2:
      digitalWrite(GREEN_LED, HIGH);     // both = "yellow"
      digitalWrite(RED_LED, HIGH);
      stage = 3;
      break;
    case 3:
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, HIGH);
      stage = 1;
      break;
  }
  delay(2000);
}
