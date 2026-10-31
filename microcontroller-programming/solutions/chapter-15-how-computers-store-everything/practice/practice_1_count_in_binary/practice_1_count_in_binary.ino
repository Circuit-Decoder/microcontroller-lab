// Chapter 15, Practice 1 - Count in Binary

void setup() {
  Serial.begin(115200);
  delay(1000);
  for (int n = 0; n <= 15; n++) {
    String bits = String(n, BIN);      // 5 -> "101"
    while (bits.length() < 4) {
      bits = "0" + bits;               // -> "0101"
    }
    Serial.print(n);
    Serial.print("\t");
    Serial.println(bits);
  }
}

void loop() {
}
