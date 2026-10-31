// Chapter 7, Practice 2 - Beeps
// beep() plays one note; happy and sad sounds are built from it.

const int BUZZER_PIN = 4;

void setup() {
  happySound();
  delay(1500);
  sadSound();
}

void loop() {
}

void beep(int frequency, int ms) {
  tone(BUZZER_PIN, frequency, ms);
  delay(ms + 30);                  // wait for the note, plus a tiny gap
}

void happySound() {                // rising notes
  beep(1000, 100);
  beep(1500, 100);
  beep(2000, 200);
}

void sadSound() {                  // falling, slower notes
  beep(600, 300);
  beep(400, 300);
  beep(250, 600);
}
