// Chapter 12, Practice 4 - Times to Halve

void setup() {
  Serial.begin(115200);
  delay(1000);
  unsigned long n = 1000000;
  int halvings = 0;
  while (n > 0) {
    n = n / 2;                // whole-number division
    halvings++;
  }
  Serial.print("Halvings to reach 0: ");
  Serial.println(halvings);   // 20
}

void loop() {
}
