// Chapter 10, Practice 3 - Light Diary
// One reading every 10 s for 5 minutes (30 readings).
// Open Tools > Serial Plotter to see the curve.

const int LDR_PIN = 35;

void setup() {
  Serial.begin(115200);
  delay(1000);
  for (int i = 0; i < 30; i++) {           // 30 x 10 s = 5 minutes
    Serial.println(analogRead(LDR_PIN));   // just the number: plottable
    delay(10000);
  }
}

void loop() {
}
