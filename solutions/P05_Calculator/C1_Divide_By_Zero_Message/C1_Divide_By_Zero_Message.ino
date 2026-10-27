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

// ---------- NEW: CALCULATOR STATE ----------
String currentInput = "";
double num1 = 0;
double num2 = 0;
char operation = 0;
int stage = 1;   // 1 = typing num1, 2 = typing num2, 3 = showing result

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

  if (key == '*') {                // * clears everything, at any time
    resetCalculator();
    return;
  }

  if (stage == 1) {                // typing the first number
    if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
      num1 = currentInput.toDouble();
      operation = mapKeyToOperation(key);
      currentInput = "";
      stage = 2;
    } else if (key >= '0' && key <= '9') {
      currentInput += key;
    }
  }
  else if (stage == 2) {           // typing the second number
    if (key == '#') {
      num2 = currentInput.toDouble();
      stage = 3;
    } else if (key >= '0' && key <= '9') {
      currentInput += key;
    }
  }
  else if (stage == 3) {           // any key after the result: start again
    resetCalculator();
    return;
  }
  showScreen();
}

char mapKeyToOperation(char key) {
  if (key == 'A') return '+';
  if (key == 'B') return '-';
  if (key == 'C') return '*';
  if (key == 'D') return '/';
  return '+';
}

double calculateResult() {
  switch (operation) {
    case '+': return num1 + num2;
    case '-': return num1 - num2;
    case '*': return num1 * num2;
    case '/':
      if (num2 == 0) return 0;     // never divide by zero!
      return num1 / num2;
  }
  return 0;
}

void resetCalculator() {
  currentInput = "";
  num1 = 0;
  num2 = 0;
  operation = 0;
  stage = 1;
  showScreen();
}

void showScreen() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);

  if (stage == 1) {
    display.println("Num 1:");
    display.println(currentInput);
  }
  else if (stage == 2) {
    display.print(num1, 0);
    display.print(" ");
    display.println(operation);
    display.println(currentInput);
  }
  else if (stage == 3) {
    // CHALLENGE 1: a clear message instead of 0
    if (operation == '/' && num2 == 0) {
      display.setTextSize(1);
      display.println("Cannot divide");
      display.println("by 0");
    } else {
      display.println("Result:");
      display.println(calculateResult(), 2);
    }
  }

  display.setTextSize(1);
  display.println("");
  display.println("* = Clear");
  display.display();
}
