// Chapter 16, Practice 1 - Best Student

const int STUDENTS = 3;
const int SUBJECTS = 4;
int marks[STUDENTS][SUBJECTS] = {
  {72, 65, 80, 91},
  {58, 77, 69, 84},
  {90, 88, 95, 79}
};

void setup() {
  Serial.begin(115200);
  delay(1000);
  int best = 0;            // index of the best student so far
  int bestTotal = -1;
  for (int s = 0; s < STUDENTS; s++) {
    int total = 0;
    for (int j = 0; j < SUBJECTS; j++) {
      total += marks[s][j];
    }
    if (total > bestTotal) {
      bestTotal = total;
      best = s;
    }
  }
  Serial.print("Best: student ");
  Serial.print(best);
  Serial.print(" with ");
  Serial.println(bestTotal);   // student 2 with 352
}

void loop() {
}
