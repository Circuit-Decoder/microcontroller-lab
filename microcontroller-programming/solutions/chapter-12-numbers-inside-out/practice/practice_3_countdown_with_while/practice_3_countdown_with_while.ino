// Chapter 12, Practice 3 - Countdown with while

void setup() {
  Serial.begin(115200);
  delay(1000);

  int i = 10;                 // start
  while (i >= 1) {            // keep going while
    Serial.println(i);
    i--;                      // step - easy to forget!
  }
  Serial.println("Lift off!");
}

void loop() {
}
