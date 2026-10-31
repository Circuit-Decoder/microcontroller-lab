// Chapter 9, Practice 1 - Times Table

void setup() {
  Serial.begin(115200);
  delay(1000);
  for (int i = 1; i <= 10; i++) {
    Serial.print("7 x ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(7 * i);
  }
}

void loop() {
}
