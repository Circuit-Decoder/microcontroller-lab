// Chapter 13, Practice 4 - Close Enough

void setup() {
  Serial.begin(115200);
  delay(1000);
  double x = 0.1 + 0.2;
  double y = 0.3;
  Serial.print("x == y? ");
  Serial.println(x == y ? "yes" : "no");          // no!
  Serial.print("nearlyEqual(x, y)? ");
  Serial.println(nearlyEqual(x, y) ? "yes" : "no"); // yes
  Serial.println(x, 20);                           // see why
}

void loop() {
}

bool nearlyEqual(double x, double y) {
  const double TOLERANCE = 0.000001;
  return fabs(x - y) < TOLERANCE;
}
