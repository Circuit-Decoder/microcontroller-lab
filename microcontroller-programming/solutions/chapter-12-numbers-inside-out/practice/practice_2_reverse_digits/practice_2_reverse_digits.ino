// Chapter 12, Practice 2 - Reverse Digits

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println(reverseNumber(12345));   // 54321
  Serial.println(reverseNumber(1200));    // 21 (zeros at the front vanish)
}

void loop() {
}

unsigned long reverseNumber(unsigned long n) {
  unsigned long reversed = 0;
  while (n > 0) {
    int lastDigit = n % 10;            // 12345 -> 5
    reversed = reversed * 10 + lastDigit;   // 0 -> 5 -> 54 -> ...
    n = n / 10;                        // 12345 -> 1234
  }
  return reversed;
}
