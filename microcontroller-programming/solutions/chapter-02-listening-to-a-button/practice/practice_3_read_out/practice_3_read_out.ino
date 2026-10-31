// Chapter 2, Practice 3 - Read-out
// E2.5 plus a print of Button B's reading ten times a second.

const int BUTTON_A  = 15;
const int BUTTON_B  = 23;
const int GREEN_LED = 18;
const int RED_LED   = 19;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
}

void loop() {
  digitalWrite(GREEN_LED, !digitalRead(BUTTON_A));
  digitalWrite(RED_LED, !digitalRead(BUTTON_B));
  Serial.println(digitalRead(BUTTON_B));   // 1 = released, 0 = pressed
  delay(100);
}
