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

// ---------- NEW: QUADRATIC SOLVER STATE ----------
// Solves  a x^2 + b x + c = 0
String currentInput = "";
double a = 0, b = 0, c = 0;
int stage = 1;   // 1 = typing a, 2 = typing b, 3 = typing c, 4 = result

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

  if (key == '*' || stage == 4) {      // * clears; any key after a result too
    resetSolver();
    return;
  }

  if (key >= '0' && key <= '9') {
    if (currentInput.length() < 8) currentInput += key;
  }
  else if (key == 'B') {               // B = minus sign, only at the start
    if (currentInput.length() == 0) currentInput = "-";
  }
  else if (key == 'C') {               // C = decimal point, only once
    if (currentInput.indexOf('.') == -1) currentInput += '.';
  }
  else if (key == 'D') {               // D = delete the last character
    if (currentInput.length() > 0) {
      currentInput.remove(currentInput.length() - 1);
    }
  }
  else if (key == '#') {               // # = this number is finished
    double value = currentInput.toDouble();
    if (stage == 1) a = value;
    if (stage == 2) b = value;
    if (stage == 3) c = value;
    currentInput = "";
    stage++;
  }
  showScreen();
}

void showScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);

  if (stage <= 3) {
    display.println("Solve ax^2+bx+c=0");
    display.println("");
    if (stage == 1) display.print("a = ");
    if (stage == 2) display.print("b = ");
    if (stage == 3) display.print("c = ");
    display.setTextSize(2);
    display.println(currentInput);
    display.setTextSize(1);
    display.println("");
    display.println("B:-  C:.  D:Del");
    display.println("# = OK   * = Reset");
  } else {
    solve();
  }
  display.display();
}

// CHALLENGE 1: 45 instead of 45.00, but keep 2.5 as 2.5
String num(double v) {
  if (v == (long)v) return String((long)v);
  return String(v, 2);
}

void solve() {
  // CHALLENGE 1: show the equation first, like 1x^2 + -5x + 6 = 0
  display.println(num(a) + "x^2 + " + num(b) + "x + " + num(c) + " = 0");
  if (a == 0) {                         // not a quadratic equation
    display.println("a = 0: this is not");
    display.println("a quadratic equation");
    if (b != 0) {
      display.println("");
      display.println("Linear answer:");
      display.print("x = ");
      display.println(-c / b, 3);
    }
    return;
  }

  double D = b * b - 4 * a * c;         // the discriminant
  if (fabs(D) < 0.000001) D = 0;        // tiny rounding error: treat as 0
  display.print("D = b^2-4ac = ");
  display.println(D, 2);

  if (D == 0) {                         // one root
    double x = -b / (2 * a);
    display.println("One real root:");
    display.print("x = ");
    display.println(x, 3);
  }
  else if (D > 0) {                     // two different real roots
    double x1 = (-b + sqrt(D)) / (2 * a);
    double x2 = (-b - sqrt(D)) / (2 * a);
    display.println("Two real roots:");
    display.print("x1 = ");
    display.println(x1, 3);
    display.print("x2 = ");
    display.println(x2, 3);
  }
  else {                                // D < 0
    display.println("No real roots");
    display.println("(D is negative)");
  }
  display.println("");
  display.println("Any key = new");
}

void resetSolver() {
  currentInput = "";
  a = 0;
  b = 0;
  c = 0;
  stage = 1;
  showScreen();
}
