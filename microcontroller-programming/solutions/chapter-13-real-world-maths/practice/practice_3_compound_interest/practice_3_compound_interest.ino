// Chapter 13, Practice 3 - Compound Interest

void setup() {
  Serial.begin(115200);
  delay(1000);
  double amount = 1000 * pow(1.05, 10);
  Serial.print("After 10 years: ");
  Serial.print(amount, 2);
  Serial.println(" taka");                 // 1628.89

  // the same, year by year
  double money = 1000;
  for (int year = 1; year <= 10; year++) {
    money = money * 1.05;
    Serial.print("Year ");
    Serial.print(year);
    Serial.print(": ");
    Serial.println(money, 2);
  }
}

void loop() {
}
