// Chapter 16, Practice 4 - Odd Only

void setup() {
  Serial.begin(115200);
  delay(1000);
  for (int i = 1; i <= 20; i++) {
    if (i % 2 == 0) {
      continue;              // even: skip the rest, go to the next i
    }
    Serial.println(i);
  }
}

void loop() {
}
