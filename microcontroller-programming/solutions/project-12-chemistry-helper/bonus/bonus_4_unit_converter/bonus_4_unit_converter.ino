// Project 12, Bonus 4 - Unit Converter
// C then 1-6 picks a conversion; type a number (C = point), # converts.

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

// Rounded atomic masses, as in the school chemistry book
String elementNames[10] = {
  "Hydrogen", "Carbon", "Nitrogen", "Oxygen", "Sodium",
  "Magnesium", "Aluminium", "Sulfur", "Chlorine", "Calcium"
};
String elementSymbols[10] = {
  "H", "C", "N", "O", "Na", "Mg", "Al", "S", "Cl", "Ca"
};
int atomicNumbers[10]   = {1, 6, 7, 8, 11, 12, 13, 16, 17, 20};
double atomicMasses[10] = {1, 12, 14, 16, 23, 24, 27, 32, 35.5, 40};

String formulas[9] = {
  "H2O", "CO2", "NaCl", "H2SO4", "NaOH", "CaCO3", "C6H12O6", "NH3", "HCl"
};
String compoundNames[9] = {
  "Water", "Carbon dioxide", "Table salt", "Sulfuric acid",
  "Caustic soda", "Limestone", "Glucose", "Ammonia", "Hydrogen chloride"
};
// Each row is a compound; each number is an element index. -1 = empty.
int compoundElements[9][3] = {
  {0, 3, -1},   // H2O
  {1, 3, -1},   // CO2
  {4, 8, -1},   // NaCl
  {0, 7,  3},   // H2SO4
  {4, 3,  0},   // NaOH
  {9, 1,  3},   // CaCO3
  {1, 0,  3},   // C6H12O6
  {2, 0, -1},   // NH3
  {0, 8, -1}    // HCl
};
int compoundCounts[9][3] = {
  {2, 1, 0}, {1, 2, 0}, {1, 1, 0}, {2, 1, 4}, {1, 1, 1},
  {1, 1, 3}, {6, 12, 6}, {1, 3, 0}, {1, 1, 0}
};

// The same "lookup + formula" idea: answer = (x - before) * factor + after
String convNames[6] = {
  "km -> miles", "miles -> km", "kg -> pounds",
  "pounds -> kg", "C -> F", "F -> C"
};
String convUnits[6] = {"miles", "km", "lb", "kg", "F", "C"};
double before[6] = {0, 0, 0, 0, 0, 32};
double factor[6] = {0.621, 1 / 0.621, 2.205, 1 / 2.205, 9.0 / 5, 5.0 / 9};
double after[6]  = {0, 0, 0, 0, 32, 0};
String typed = "";
bool converted = false;

int stage = 0;      // 0 = menu, 1 = elements, 2 = compounds, 3 = units
int shown = -1;     // the element or compound on screen, -1 = the list

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

  if (key == '*') {
    stage = 0;
    shown = -1;
    typed = "";
    converted = false;
  } else if (stage == 3) {
    unitKey(key);
  } else if (stage == 0) {
    if (key == 'A') stage = 1;
    if (key == 'B') stage = 2;
    if (key == 'C') stage = 3;
  } else if (stage == 1 && key >= '0' && key <= '9') {
    shown = key - '0';
  } else if (stage == 2 && key >= '1' && key <= '9') {
    shown = key - '1';                // key 1 = compound 0
  }
  showScreen();
}

// Prints 18 as "18" but 35.5 as "35.5"
void printNumber(double v) {
  if (v == (long)v) display.print((long)v);
  else display.print(v, 1);
}

void showScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  if (stage == 0) {
    display.println("Chemistry Helper");
    display.setCursor(0, 24);
    display.println("A: Element info");
    display.println("B: Molar mass");
    display.println("C: Unit converter");
  } else if (stage == 1 && shown == -1) {
    display.println("Element: press 0-9");
    display.println("*=menu");
    display.println("0:H 1:C 2:N 3:O 4:Na");
    display.println("5:Mg 6:Al 7:S 8:Cl");
    display.println("9:Ca");
  } else if (stage == 3) {
    showUnits();
  } else if (stage == 1) {
    showElement(shown);
  } else if (shown == -1) {
    display.println("Compound: press 1-9");
    display.println("*=menu");
    display.println("1:H2O 2:CO2 3:NaCl");
    display.println("4:H2SO4 5:NaOH");
    display.println("6:CaCO3 7:C6H12O6");
    display.println("8:NH3 9:HCl");
  } else {
    showCompound(shown);
  }
  display.display();
}

void showElement(int i) {
  display.setTextSize(2);
  display.println(elementSymbols[i]);
  display.setTextSize(1);
  display.println(elementNames[i]);
  display.print("Atomic number: ");
  display.println(atomicNumbers[i]);
  display.print("Atomic mass: ");
  printNumber(atomicMasses[i]);
  display.println();
  display.setCursor(0, 56);
  display.print("0-9=another *=menu");
}

void showCompound(int c) {
  display.print(formulas[c]);
  display.print(" ");
  display.println(compoundNames[c]);
  display.setCursor(0, 16);
  double total = 0;
  for (int col = 0; col < 3; col++) {
    int e = compoundElements[c][col];
    int n = compoundCounts[c][col];
    if (e == -1) continue;            // empty place: skip it
    double part = n * atomicMasses[e];
    total += part;
    display.print(elementSymbols[e]);  // e.g. "O: 4 x 16 = 64"
    display.print(": ");
    display.print(n);
    display.print(" x ");
    printNumber(atomicMasses[e]);
    display.print(" = ");
    printNumber(part);
    display.println();
  }
  display.setCursor(0, 48);
  display.print("Total: ");
  printNumber(total);
  display.println(" g/mol");
}

void unitKey(char key) {
  if (shown == -1) {                         // choosing a conversion
    if (key >= '1' && key <= '6') shown = key - '1';
    return;
  }
  if (converted) {                           // any key: next number
    typed = "";
    converted = false;
  }
  if (key >= '0' && key <= '9' && typed.length() < 8) {
    typed += key;
  } else if (key == 'C' && typed.indexOf('.') == -1) {
    typed += '.';
  } else if (key == 'D' && typed.length() > 0) {
    typed.remove(typed.length() - 1);
  } else if (key == 'B' && typed.length() == 0) {
    typed = "-";                             // for temperatures
  } else if (key == '#' && typed.length() > 0) {
    converted = true;
  }
}

void showUnits() {
  if (shown == -1) {
    display.println("Units: press 1-6");
    display.println("*=menu");
    for (int i = 0; i < 6; i++) {
      display.print(i + 1);
      display.print(": ");
      display.println(convNames[i]);
    }
    return;
  }
  display.println(convNames[shown]);
  display.println("C=. D=del B=- #=go");
  display.setCursor(0, 24);
  display.setTextSize(2);
  display.println(typed);
  if (converted) {
    double x = typed.toDouble();
    double answer = (x - before[shown]) * factor[shown] + after[shown];
    display.print("=");
    display.print(answer, 2);
    display.setTextSize(1);
    display.print(" ");
    display.print(convUnits[shown]);
  }
}
