// Chapter 1, Practice 1 - SOS
// The green LED flashes S-O-S in Morse code, then waits 2 seconds.

const int GREEN_LED = 18;   // green LED + 220 ohm on GPIO 18

void setup() {
  pinMode(GREEN_LED, OUTPUT);
}

void loop() {
  // S: three short flashes (200 ms on, 200 ms off)
  digitalWrite(GREEN_LED, HIGH); delay(200);
  digitalWrite(GREEN_LED, LOW);  delay(200);
  digitalWrite(GREEN_LED, HIGH); delay(200);
  digitalWrite(GREEN_LED, LOW);  delay(200);
  digitalWrite(GREEN_LED, HIGH); delay(200);
  digitalWrite(GREEN_LED, LOW);  delay(200);
  // O: three long flashes (600 ms on, 200 ms off)
  digitalWrite(GREEN_LED, HIGH); delay(600);
  digitalWrite(GREEN_LED, LOW);  delay(200);
  digitalWrite(GREEN_LED, HIGH); delay(600);
  digitalWrite(GREEN_LED, LOW);  delay(200);
  digitalWrite(GREEN_LED, HIGH); delay(600);
  digitalWrite(GREEN_LED, LOW);  delay(200);
  // S: three short flashes again
  digitalWrite(GREEN_LED, HIGH); delay(200);
  digitalWrite(GREEN_LED, LOW);  delay(200);
  digitalWrite(GREEN_LED, HIGH); delay(200);
  digitalWrite(GREEN_LED, LOW);  delay(200);
  digitalWrite(GREEN_LED, HIGH); delay(200);
  digitalWrite(GREEN_LED, LOW);  delay(200);
  delay(2000);                        // pause before the next SOS
}
