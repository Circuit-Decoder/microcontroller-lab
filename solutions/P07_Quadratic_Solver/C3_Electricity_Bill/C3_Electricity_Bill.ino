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

// CHALLENGE 3: Electricity bill with slabs
// The typing part is the same as the Quadratic Solver:
// B = minus, C = decimal point, D = delete, # = OK, * = reset
String currentInput = "";
double values[1];
const int NUM_VALUES = 1;
int stage = 1;   // 1..1 = typing a value, 2 = result

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
    if (stage == 1) display.print("Units used = ");
    display.setTextSize(2);
    display.println(currentInput);
    display.setTextSize(1);
    display.setCursor(0, 48);
    display.println("D:Del");
    display.println("# = OK   * = Reset");
  } else {
    showResult();
  }
  display.display();
}

void showResult() {
  // EXAMPLE slab rates in taka per unit. Copy the real ones from your
  // family's electricity bill - they change from time to time!
  // Slab:           1-75   76-200  201-300  301-400  401-600  600+
  double limits[] = {75,    200,    300,     400,     600,     1e9};
  double rates[]  = {5.26,  7.20,   7.59,    7.95,    12.51,   14.61};

  double units = values[0];
  double bill = 0;
  double previousLimit = 0;
  for (int i = 0; i < 6; i++) {
    if (units > previousLimit) {
      double inThisSlab = min(units, limits[i]) - previousLimit;   // units in this slab
      bill += inThisSlab * rates[i];
    }
    previousLimit = limits[i];
  }
  display.print("Units: ");
  display.println(units, 0);
  display.println("Energy charge:");
  display.setTextSize(2);
  display.print(bill, 2);
  display.setTextSize(1);
  display.println(" tk");
  display.println("(+ demand charge, VAT)");
  display.setCursor(0, 56);
  display.print("Any key = new");
}
