// Project 07, Bonus 2 - Keep Going
// After an answer, A-D carry it on as the first number.
// Stage 1: type the first number, then A(+) B(-) C(*) D(/).
// Stage 2: type the second number, then # (=).  Stage 3: the answer.
// * clears at any time. Any key after an answer starts again.

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int SCREEN_WIDTH  = 128;
const int SCREEN_HEIGHT = 64;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int stage = 1;
String typed = "";
double num1 = 0;
double num2 = 0;
char operation = ' ';

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
  }
  display.setTextColor(SSD1306_WHITE);
  showScreen();
}

void loop() {
  char key = keypad.getKey();
  if (!key) return;

  bool divError = (operation == '/' && num2 == 0);
  if (stage == 3 && isOperationKey(key) && !divError) {
    num1 = calculate();                    // the answer carries on
    operation = toSymbol(key);
    typed = "";
    stage = 2;
    showScreen();
    return;
  }

  if (key == '*' || stage == 3) {          // clear, or start again
    resetCalculator();
    showScreen();
    return;
  }

  if (key >= '0' && key <= '9') {
    if (typed.length() < 9) {
      typed += key;
    }
  } else if (stage == 1 && typed.length() > 0 && isOperationKey(key)) {
    num1 = typed.toDouble();
    operation = toSymbol(key);
    typed = "";
    stage = 2;
  } else if (stage == 2 && typed.length() > 0 && key == '#') {
    num2 = typed.toDouble();
    stage = 3;
  }
  showScreen();
}

bool isOperationKey(char k) {
  return k == 'A' || k == 'B' || k == 'C' || k == 'D';
}

char toSymbol(char k) {
  switch (k) {
    case 'A': return '+';
    case 'B': return '-';
    case 'C': return '*';
    default:  return '/';
  }
}

void resetCalculator() {
  stage = 1;
  typed = "";
  num1 = 0;
  num2 = 0;
  operation = ' ';
}

void showScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Calculator");

  if (stage == 1) {
    display.println("A+ B- C* D/  *=Clear");
    display.setTextSize(2);
    display.setCursor(0, 24);
    display.println(typed);
  } else if (stage == 2) {
    display.print(num1, 2);                // may now have decimals
    display.print(" ");
    display.print(operation);
    display.println(" then #");
    display.setTextSize(2);
    display.setCursor(0, 24);
    display.println(typed);
  } else {
    display.print(num1, 0);
    display.print(" ");
    display.print(operation);
    display.print(" ");
    display.print(num2, 0);
    display.println(" =");
    display.setTextSize(2);
    display.setCursor(0, 24);
    if (operation == '/' && num2 == 0) {
      display.println("Error:");
      display.println("div by 0");
    } else {
      display.println(calculate(), 2);
    }
  }
  display.display();
}

double calculate() {
  switch (operation) {
    case '+': return num1 + num2;
    case '-': return num1 - num2;
    case '*': return num1 * num2;
    default:  return num1 / num2;   // only called when num2 is not 0
  }
}
