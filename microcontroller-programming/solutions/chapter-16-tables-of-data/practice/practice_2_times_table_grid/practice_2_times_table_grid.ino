// Chapter 16, Practice 2 - Times Table

int table[10][10];

void setup() {
  Serial.begin(115200);
  delay(1000);
  for (int r = 0; r < 10; r++) {           // fill it...
    for (int c = 0; c < 10; c++) {
      table[r][c] = (r + 1) * (c + 1);
    }
  }
  for (int r = 0; r < 10; r++) {           // ...then print it
    for (int c = 0; c < 10; c++) {
      Serial.print(table[r][c]);
      Serial.print("\t");                  // tab keeps columns lined up
    }
    Serial.println();
  }
}

void loop() {
}
