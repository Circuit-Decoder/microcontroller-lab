// Chapter 15, Practice 3 - Bits Needed

void setup() {
  Serial.begin(115200);
  delay(1000);
  unsigned long tests[6] = {1, 25, 255, 256, 1000, 1000000};
  for (int i = 0; i < 6; i++) {
    Serial.print(tests[i]);
    Serial.print(" needs ");
    Serial.print(bitsNeeded(tests[i]));
    Serial.println(" bits");
  }
}

void loop() {
}

int bitsNeeded(unsigned long n) {
  if (n == 0) return 1;          // "0" still needs one bit
  int bits = 0;
  while (n > 0) {
    n = n / 2;                   // each halving removes one bit
    bits++;
  }
  return bits;
}
