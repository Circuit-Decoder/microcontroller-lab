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

// CHALLENGE 4: Savings growth with compound interest
// The typing part is the same as the Quadratic Solver:
// B = minus, C = decimal point, D = delete, # = OK, * = reset
String currentInput = "";
double values[3];
const int NUM_VALUES = 3;
int stage = 1;   // 1..3 = typing a value, 4 = result

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

  if (key == '*' || stage > NUM_VALUES) {   // * clears; any key after a result too
    currentInput = "";
    stage = 1;
    showScreen();
    return;
  }
  if (key >= '0' && key <= '9') {
    if (currentInput.length() < 8) currentInput += key;
  }
  else if (key == 'B') {
    if (currentInput.length() == 0) currentInput = "-";
  }
  else if (key == 'C') {
    if (currentInput.indexOf('.') == -1) currentInput += '.';
  }
  else if (key == 'D') {
    if (currentInput.length() > 0) currentInput.remove(currentInput.length() - 1);
  }
  else if (key == '#') {
    values[stage - 1] = currentInput.toDouble();
    currentInput = "";
    stage++;
  }
  showScreen();
}

void showScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  if (stage <= NUM_VALUES) {
    if (stage == 1) display.print("Money = ");
    if (stage == 2) display.print("Rate % = ");
    if (stage == 3) display.print("Years = ");
    display.setTextSize(2);
    display.println(currentInput);
    display.setTextSize(1);
    display.setCursor(0, 48);
    display.println("B:-  C:.  D:Del");
    display.println("# = OK   * = Reset");
  } else {
    showResult();
  }
  display.display();
}

void showResult() {
  double P = values[0];   // starting money
  double r = values[1];   // interest rate per year, like 6
  double n = values[2];   // number of years
  double amount = P * pow(1 + r / 100.0, n);   // remember the .0!
  display.print(P, 0);
  display.print(" tk at ");
  display.print(r, 1);
  display.println("%");
  display.print("for ");
  display.print(n, 0);
  display.println(" years becomes:");
  display.setTextSize(2);
  display.println(amount, 2);
  display.setTextSize(1);
  display.print("Interest: ");
  display.println(amount - P, 2);
  display.setCursor(0, 56);
  display.print("Any key = new");
}
