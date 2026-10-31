// Chapter 7, Practice 3 - Grade
// grade() turns a mark into a letter grade.

void setup() {
  Serial.begin(115200);
  delay(1000);
  int marks[5] = {92, 75, 63, 41, 30};
  for (int i = 0; i < 5; i++) {
    Serial.print(marks[i]);
    Serial.print(" -> ");
    Serial.println(grade(marks[i]));
  }
}

void loop() {
}

String grade(int mark) {
  if (mark >= 80) return "A+";     // checked from the top down,
  if (mark >= 70) return "A";      // so 75 never reaches the
  if (mark >= 60) return "A-";     // lines below
  if (mark >= 50) return "B";
  if (mark >= 40) return "C";
  if (mark >= 33) return "D";
  return "F";
}
