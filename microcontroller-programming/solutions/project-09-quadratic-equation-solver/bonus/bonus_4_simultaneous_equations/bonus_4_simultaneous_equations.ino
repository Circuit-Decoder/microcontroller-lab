// Project 09, Bonus 4 - Simultaneous Equations
// A switches mode: quadratic, or a1x + b1y = c1, a2x + b2y = c2.
// Solve a*x^2 + b*x + c = 0. Type a, then #, b, then #, c, then #.
// B = minus (first only), C = decimal point (once), D = delete, * = new.

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

const double TINY = 0.000001;

int stage = 0;           // 0 = typing a, 1 = b, 2 = c, 3 = result
bool linearMode = false;     // A switches between the two modes
double coef[6];
String labels[6] = {"a1", "b1", "c1", "a2", "b2", "c2"};

int needed() {               // how many numbers this mode needs
  return linearMode ? 6 : 3;
}
String typed = "";

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
  }
  display.setTextColor(SSD1306_WHITE);
  showEntry();
}

void loop() {
  char key = keypad.getKey();
  if (!key) return;

  if (key == 'A' && stage == 0 && typed.length() == 0) {
    linearMode = !linearMode;
    showEntry();
    return;
  }
  if (key == '*' || stage == needed()) {
    stage = 0;
    typed = "";
    showEntry();
    return;
  }
  if (key >= '0' && key <= '9') {
    if (typed.length() < 8) typed += key;
  } else if (key == 'B') {
    if (typed.length() == 0) typed = "-";
  } else if (key == 'C') {
    if (typed.indexOf('.') == -1 && typed.length() < 8) typed += '.';
  } else if (key == 'D') {
    if (typed.length() > 0) typed.remove(typed.length() - 1);
  } else if (key == '#') {
    if (!isValidNumber(typed)) return;   // ignore "", "-", ".", "-."
    coef[stage] = typed.toDouble();
    typed = "";
    stage++;
    if (stage == needed()) {
      if (linearMode) {
        solveLinear();
      } else {
        solve(coef[0], coef[1], coef[2]);
      }
      return;
    }
  }
  showEntry();
}

// A valid number has at least one digit in it.
bool isValidNumber(String s) {
  for (int i = 0; i < s.length(); i++) {
    if (s[i] >= '0' && s[i] <= '9') return true;
  }
  return false;
}

void showEntry() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  if (linearMode) {
    display.println("ax + by = c  (two)");
  } else {
    display.println("ax^2 + bx + c = 0");
  }
  display.println("B:-  C:.  D:del  #:ok");
  display.setCursor(0, 24);
  display.print("Type ");
  if (linearMode) {
    display.print(labels[stage]);
  } else {
    display.print(labels[stage][0]);  // just a, b or c
  }
  display.print(":");
  if (stage == 0) display.print("   A=mode");
  display.println();
  display.setTextSize(2);
  display.setCursor(0, 40);
  display.println(typed);
  display.display();
}

void solve(double a, double b, double c) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Result");
  display.println("Any key = new");
  display.setCursor(0, 16);

  if (fabs(a) < TINY) {                       // not quadratic
    if (fabs(b) < TINY) {
      display.println("a and b are 0:");
      display.println("not an equation.");
    } else {
      display.println("a = 0: linear");
      display.print("x = ");
      display.println(-c / b, 4);
    }
    display.display();
    return;
  }

  double D = b * b - 4 * a * c;
  if (fabs(D) < TINY) D = 0;
  display.print("D = ");
  display.println(D, 4);

  if (D > 0) {
    display.println("Two real roots:");
    display.print("x1 = ");
    display.println((-b + sqrt(D)) / (2 * a), 4);
    display.print("x2 = ");
    display.println((-b - sqrt(D)) / (2 * a), 4);
  } else if (D == 0) {
    display.println("One real root:");
    display.print("x = ");
    display.println(-b / (2 * a), 4);
  } else {
    display.println("No real roots");
    display.println("(D is negative)");
  }
  display.display();
}

// Cramer's rule: a1x + b1y = c1 and a2x + b2y = c2
void solveLinear() {
  double a1 = coef[0], b1 = coef[1], c1 = coef[2];
  double a2 = coef[3], b2 = coef[4], c2 = coef[5];
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Result");
  display.println("Any key = new");
  display.setCursor(0, 16);

  double det = a1 * b2 - a2 * b1;
  if (fabs(det) < TINY) {
    display.println("No single answer:");
    display.println("the lines are");
    display.println("parallel or the same.");
  } else {
    double x = (c1 * b2 - c2 * b1) / det;
    double y = (a1 * c2 - a2 * c1) / det;
    display.print("x = ");
    display.println(x, 4);
    display.print("y = ");
    display.println(y, 4);
  }
  display.display();
}
