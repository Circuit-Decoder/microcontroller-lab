// Chapter 12, Practice 1 - Prime List
// Every prime from 2 to 100, using E12.3's firstFactor().

void setup() {
  Serial.begin(115200);
  delay(1000);
  int count = 0;
  for (unsigned long n = 2; n <= 100; n++) {
    if (firstFactor(n) == 0) {        // no factor found: prime
      Serial.print(n);
      Serial.print(" ");
      count++;
    }
  }
  Serial.println();
  Serial.print(count);
  Serial.println(" primes");          // 25
}

void loop() {
}

// E12.3's function, with the "tries" printing taken out
unsigned long firstFactor(unsigned long n) {
  unsigned long d = 2;
  while (d * d <= n) {
    if (n % d == 0) {
      return d;
    }
    d++;
  }
  return 0;
}
