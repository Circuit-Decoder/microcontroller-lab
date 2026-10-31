// Chapter 3, Practice 1 - Seconds and Minutes
// E3.1's counter, also shown as minutes and seconds (like 2 min 5 s).

int count = 0;   // seconds since the start

void setup() {
  Serial.begin(115200);
}

void loop() {
  int minutes = count / 60;   // whole minutes
  int seconds = count % 60;   // the seconds left over

  Serial.print("count is ");
  Serial.print(count);
  Serial.print("  =  ");
  Serial.print(minutes);
  Serial.print(" min ");
  Serial.print(seconds);
  Serial.println(" s");

  count = count + 1;
  delay(1000);
}
