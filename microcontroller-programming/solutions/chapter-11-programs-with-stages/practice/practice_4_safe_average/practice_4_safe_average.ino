// Chapter 11, Practice 4 - Safe Average

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println(average(250, 4));   // 62.50
  Serial.println(average(250, 0));   // warning, then 0.00
}

void loop() {
}

double average(double total, int count) {
  if (count == 0) {
    Serial.println("Warning: no values to average");
    return 0;
  }
  return total / count;
}
