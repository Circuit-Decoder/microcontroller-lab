// Chapter 1, Practice 4 - Speed Test
// E1.4 with the delay in one named box, so it is easy to try values.
// Upload, look, then make BLINK_MS smaller and upload again.

const int GREEN_LED = 18;
const int BLINK_MS  = 20;             // try 200, 100, 50, 20, 10, 5 ...

void setup() {
  pinMode(GREEN_LED, OUTPUT);
}

void loop() {
  digitalWrite(GREEN_LED, HIGH);
  delay(BLINK_MS);
  digitalWrite(GREEN_LED, LOW);
  delay(BLINK_MS);
}
