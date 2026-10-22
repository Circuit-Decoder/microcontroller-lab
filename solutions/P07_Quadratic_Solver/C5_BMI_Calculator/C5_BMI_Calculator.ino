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

// CHALLENGE 5: BMI calculator
// The typing part is the same as the Quadratic Solver:
// B = minus, C = decimal point, D = delete, # = OK, * = reset
String currentInput = "";
double values[2];
const int NUM_VALUES = 2;
int stage = 1;   // 1..2 = typing a value, 3 = result

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
    if (stage == 1) display.print("Height cm = ");
    if (stage == 2) display.print("Weight kg = ");
    display.setTextSize(2);
    display.println(currentInput);
    display.setTextSize(1);
    display.setCursor(0, 48);
    display.println("C:.  D:Del");
    display.println("# = OK   * = Reset");
  } else {
    showResult();
  }
  display.display();
}

void showResult() {
  double heightM = values[0] / 100.0;   // cm -> metres
  double weight = values[1];
  if (heightM <= 0) {
    display.println("Height must be");
    display.println("above 0");
    return;
  }
  double bmi = weight / (heightM * heightM);
  display.println("BMI =");
  display.setTextSize(3);
  display.println(bmi, 1);
  display.setTextSize(1);
  display.println("For adults only -");
  display.println("practise coding!");
  display.setCursor(0, 56);
  display.print("Any key = new");
}
