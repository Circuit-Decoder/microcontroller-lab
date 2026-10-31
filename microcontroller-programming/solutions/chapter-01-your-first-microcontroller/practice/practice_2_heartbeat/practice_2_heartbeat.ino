// Chapter 1, Practice 2 - Heartbeat
// Two quick flashes, then a long pause, like a beating heart.

const int GREEN_LED = 18;   // green LED + 220 ohm on GPIO 18

void setup() {
  pinMode(GREEN_LED, OUTPUT);
}

void loop() {
  digitalWrite(GREEN_LED, HIGH);
  delay(100);                         // first beat
  digitalWrite(GREEN_LED, LOW);
  delay(150);
  digitalWrite(GREEN_LED, HIGH);
  delay(100);                         // second beat
  digitalWrite(GREEN_LED, LOW);
  delay(800);                         // rest before the next heartbeat
}
