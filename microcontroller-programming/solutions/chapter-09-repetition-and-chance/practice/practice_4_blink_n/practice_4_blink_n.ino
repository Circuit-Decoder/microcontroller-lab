// Chapter 9, Practice 4 - Blink N

const int GREEN_LED = 18;
const int RED_LED   = 19;

void setup() {
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
}

void loop() {
  blink(GREEN_LED, 3);
  delay(1000);
  blink(RED_LED, 5);
  delay(2000);
}

void blink(int pin, int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(200);
    digitalWrite(pin, LOW);
    delay(200);
  }
}
