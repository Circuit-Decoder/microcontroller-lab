// Chapter 10, Practice 4 - Map Backwards
// The darker the room, the brighter the green LED.

const int LDR_PIN   = 35;
const int GREEN_LED = 18;

void setup() {
  pinMode(GREEN_LED, OUTPUT);
}

void loop() {
  int light = analogRead(LDR_PIN);                  // bright = big
  int brightness = map(light, 0, 4095, 255, 0);     // reversed range
  analogWrite(GREEN_LED, brightness);
  delay(20);
}
