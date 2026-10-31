// Chapter 3, Practice 4 - Initial Letter
// A char box holds the first letter of a name; print it ten times.

char initial = 'R';   // change this to the first letter of your name

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.print(initial);
  Serial.print(initial);
  Serial.print(initial);
  Serial.print(initial);
  Serial.print(initial);
  Serial.print(initial);
  Serial.print(initial);
  Serial.print(initial);
  Serial.print(initial);
  Serial.print(initial);
  Serial.println();          // end the line
}

void loop() {
}
