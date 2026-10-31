// Chapter 2, Practice 4 - Bounce Hunt
// E2.2 reading Button A every millisecond, to catch contact bounce.

const int BUTTON_A = 15;   // Button A: GPIO 15 to GND

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_A, INPUT_PULLUP);
}

void loop() {
  Serial.println(digitalRead(BUTTON_A));   // 1 = released, 0 = pressed
  delay(1);                                // 1000 readings a second
}
