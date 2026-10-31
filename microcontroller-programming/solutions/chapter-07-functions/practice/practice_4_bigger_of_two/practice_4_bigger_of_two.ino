// Chapter 7, Practice 4 - Bigger of Two

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println(biggest(7, 12));    // 12
  Serial.println(biggest(40, -3));   // 40
  Serial.println(biggest(5, 5));     // 5
}

void loop() {
}

int biggest(int a, int b) {
  if (a > b) {
    return a;
  }
  return b;                          // b is bigger, or they are equal
}
