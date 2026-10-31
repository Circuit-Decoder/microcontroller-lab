// Chapter 16, Practice 3 - Key Finder
// Search the keypad table for a character. Type it in the Serial Monitor.

char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Type a keypad character (e.g. #) and press Enter");
}

void loop() {
  if (!Serial.available()) return;
  char wanted = Serial.read();
  if (wanted == '\n' || wanted == '\r') return;   // skip line endings

  bool found = false;
  for (int r = 0; r < 4; r++) {
    for (int c = 0; c < 4; c++) {
      if (keys[r][c] == wanted) {
        Serial.print(wanted);
        Serial.print(" is at row ");
        Serial.print(r);
        Serial.print(", column ");
        Serial.println(c);
        found = true;
      }
    }
  }
  if (!found) {
    Serial.print(wanted);
    Serial.println(" is not on the keypad");
  }
}
