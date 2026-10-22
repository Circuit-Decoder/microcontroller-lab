#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
 
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
 
// ---------- BILL SPLIT STATE ----------
String currentInput = "";
double billTotal = 0;
int numPeople = 0;
int tipPercent = 0;
int stage = 1;   // 1 = bill, 2 = people, 3 = tip, 4 = result
 
void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }
  display.setTextColor(SSD1306_WHITE);
  showScreen();
}
 
void loop() {
  char key = keypad.getKey();
  if (!key) return;
 
  if (key == '*') {
    resetCalculator();
    return;
  }
  if (key == '#') {
    advanceStage();
  }
  else if (key >= '0' && key <= '9') {
    currentInput += key;
    showScreen();
  }
}
 
void advanceStage() {
  if (stage == 1) {
    billTotal = currentInput.toDouble();
    currentInput = "";
    stage = 2;
  }
  else if (stage == 2) {
    numPeople = currentInput.toInt();
    currentInput = "";
    stage = 3;
  }
  else if (stage == 3) {
    tipPercent = currentInput.toInt();
    currentInput = "";
    stage = 4;
  }
  else if (stage == 4) {
    resetCalculator();
    return;
  }
  showScreen();
}
 
void resetCalculator() {
  currentInput = "";
  billTotal = 0;
  numPeople = 0;
  tipPercent = 0;
  stage = 1;
  showScreen();
}
 
void showScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
 
  if (stage == 1) {
    display.println("Enter bill total:");
    display.setTextSize(2);
    display.println(currentInput);
  }
  else if (stage == 2) {
    display.println("Enter # of people:");
    display.setTextSize(2);
    display.println(currentInput);
  }
  else if (stage == 3) {
    display.println("Enter tip % (0 ok):");
    display.setTextSize(2);
    display.println(currentInput);
  }
  else if (stage == 4) {
    if (numPeople <= 0) {            // never divide by zero people!
      display.println("Need at least 1");
      display.println("person!");
    } else {
      double tipAmount = billTotal * (tipPercent / 100.0);
      double totalWithTip = billTotal + tipAmount;
      double perPerson = totalWithTip / numPeople;
      display.print("Total w/ tip: ");
      display.println(totalWithTip, 2);
      display.println("");
      display.setTextSize(2);
      display.println("Each:");
      display.println(perPerson, 2);
    }
  }
 
  display.setTextSize(1);
  display.println("");
  display.println("# = Next * = Reset");
  display.display();
}
