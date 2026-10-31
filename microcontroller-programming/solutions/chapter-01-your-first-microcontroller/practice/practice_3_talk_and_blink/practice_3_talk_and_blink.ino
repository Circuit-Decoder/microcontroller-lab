// Chapter 1, Practice 3 - Talk and Blink
// E1.5 railway crossing lights, and the Serial Monitor says which is on.

const int GREEN_LED = 18;
const int RED_LED   = 19;

void setup() {
  Serial.begin(115200);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
}

void loop() {
  digitalWrite(GREEN_LED, HIGH);      // green on, red off
  digitalWrite(RED_LED, LOW);
  Serial.println("Green!");
  delay(500);

  digitalWrite(GREEN_LED, LOW);       // green off, red on
  digitalWrite(RED_LED, HIGH);
  Serial.println("Red!");
  delay(500);
}
