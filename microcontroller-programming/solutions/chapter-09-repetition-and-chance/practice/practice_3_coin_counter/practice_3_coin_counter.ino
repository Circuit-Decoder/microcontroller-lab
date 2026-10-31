// Chapter 9, Practice 3 - Coin Counter

void setup() {
  Serial.begin(115200);
  delay(1000);
  randomSeed(analogRead(34));          // a different run each time
  int heads = 0;
  for (int i = 0; i < 1000; i++) {
    if (random(0, 2) == 0) {           // 0 = heads, 1 = tails
      heads++;
    }
  }
  Serial.print("Heads: ");
  Serial.println(heads);
  Serial.print("Tails: ");
  Serial.println(1000 - heads);
}

void loop() {
}
