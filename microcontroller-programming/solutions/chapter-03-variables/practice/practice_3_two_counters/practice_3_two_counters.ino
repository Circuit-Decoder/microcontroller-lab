// Chapter 3, Practice 3 - Two Counters
// E3.5 with a second counter, looks, that goes up every loop.

const int BUTTON_A = 15;   // Button A: GPIO 15 to GND
int presses = 0;           // goes up while the button is down
int looks = 0;             // goes up every time round loop()

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_A, INPUT_PULLUP);
}

void loop() {
  presses = presses + !digitalRead(BUTTON_A);
  looks = looks + 1;
  Serial.print("presses: ");
  Serial.print(presses);
  Serial.print("   looks: ");
  Serial.println(looks);
  delay(100);   // ten checks a second
}
