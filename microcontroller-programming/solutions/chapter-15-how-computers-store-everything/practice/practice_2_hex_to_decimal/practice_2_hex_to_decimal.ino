// Chapter 15, Practice 2 - Hex to Decimal

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println(hexToDecimal("7EA"));   // 2026
  Serial.println(hexToDecimal("FF"));    // 255
  Serial.println(hexToDecimal("10"));    // 16
}

void loop() {
}

unsigned long hexToDecimal(String hex) {
  unsigned long value = 0;
  for (int i = 0; i < hex.length(); i++) {
    char c = hex[i];
    int digit;
    if (c >= '0' && c <= '9') {
      digit = c - '0';
    } else {
      digit = c - 'A' + 10;          // 'A' -> 10 ... 'F' -> 15
    }
    value = value * 16 + digit;
  }
  return value;
}
