// Chapter 14, Practice 3 - Sort Names

const int COUNT = 5;
String names[COUNT] = {"Rafi", "Anika", "Tanvir", "Mitu", "Fahim"};

void setup() {
  Serial.begin(115200);
  delay(1000);
  for (int pass = 0; pass < COUNT - 1; pass++) {
    for (int i = 0; i < COUNT - 1 - pass; i++) {
      if (names[i] > names[i + 1]) {     // later in the alphabet
        String temp = names[i];
        names[i] = names[i + 1];
        names[i + 1] = temp;
      }
    }
  }
  for (int i = 0; i < COUNT; i++) {
    Serial.println(names[i]);
  }
}

void loop() {
}
