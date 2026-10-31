// Chapter 10, Practice 2 - Traffic Light Knob
// Low = green, middle = both, high = red.

const int POT_PIN   = 34;
const int GREEN_LED = 18;
const int RED_LED   = 19;

void setup() {
  Serial.begin(115200);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
}

void loop() {
  int reading = analogRead(POT_PIN);       // 0 to 4095
  if (reading < 1365) {                    // bottom third
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  } else if (reading < 2730) {             // middle third
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, HIGH);
  } else {                                 // top third
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  }
  Serial.println(reading);
  delay(50);
}
