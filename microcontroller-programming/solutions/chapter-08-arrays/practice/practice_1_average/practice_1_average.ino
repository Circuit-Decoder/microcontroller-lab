// Chapter 8, Practice 1 - Average

int marks[5] = {72, 85, 64, 90, 78};

void setup() {
  Serial.begin(115200);
  delay(1000);
  int total = marks[0] + marks[1] + marks[2] + marks[3] + marks[4];
  float average = total / 5.0;       // 5.0, not 5: keeps the decimals
  Serial.print("Total: ");
  Serial.println(total);
  Serial.print("Average: ");
  Serial.println(average);           // 77.80
}

void loop() {
}
