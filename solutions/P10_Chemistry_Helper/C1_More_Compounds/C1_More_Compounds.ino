#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {        // <-- this is a 2D array too!
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ---------- ELEMENT DATA (parallel arrays, like Project 2) ----------
// Atomic masses are rounded the same way as in the SSC chemistry book.
String elementNames[10]   = {"Hydrogen", "Carbon", "Nitrogen", "Oxygen", "Sodium",
                             "Magnesium", "Aluminium", "Sulfur", "Chlorine", "Calcium"};
String elementSymbols[10] = {"H", "C", "N", "O", "Na", "Mg", "Al", "S", "Cl", "Ca"};
int atomicNumbers[10]     = {1, 6, 7, 8, 11, 12, 13, 16, 17, 20};
double atomicMasses[10]   = {1, 12, 14, 16, 23, 24, 27, 32, 35.5, 40};

// ---------- NEW: COMPOUND DATA (2D arrays) ----------
// CHALLENGE 1: 12 compounds now (3 new ones at the end)
const int NUM_COMPOUNDS = 12;
String formulas[NUM_COMPOUNDS] = {"H2O", "CO2", "NaCl", "H2SO4", "NaOH",
                      "CaCO3", "C6H12O6", "NH3", "HCl",
                      "HNO3", "Ca(OH)2", "MgO"};
String compoundNames[NUM_COMPOUNDS] = {"Water", "Carbon dioxide", "Table salt", "Sulfuric acid",
                           "Caustic soda", "Limestone", "Glucose", "Ammonia",
                           "Hydrogen chloride",
                           "Nitric acid", "Slaked lime", "Magnesia"};

// Each ROW is one compound. Each COLUMN is one element in it.
// The numbers are element indexes (0 = H, 1 = C, 3 = O ...). -1 = empty.
int compoundElements[NUM_COMPOUNDS][3] = {
  {0,  3, -1},   // H2O:     H, O
  {1,  3, -1},   // CO2:     C, O
  {4,  8, -1},   // NaCl:    Na, Cl
  {0,  7,  3},   // H2SO4:   H, S, O
  {4,  3,  0},   // NaOH:    Na, O, H
  {9,  1,  3},   // CaCO3:   Ca, C, O
  {1,  0,  3},   // C6H12O6: C, H, O
  {2,  0, -1},   // NH3:     N, H
  {0,  8, -1},   // HCl:     H, Cl
  {0,  2,  3},   // HNO3:    H, N, O
  {9,  3,  0},   // Ca(OH)2: Ca, O, H
  {5,  3, -1}    // MgO:     Mg, O
};
// How many atoms of each element (same rows and columns as above)
int compoundCounts[NUM_COMPOUNDS][3] = {
  {2, 1, 0},     // H2O
  {1, 2, 0},     // CO2
  {1, 1, 0},     // NaCl
  {2, 1, 4},     // H2SO4
  {1, 1, 1},     // NaOH
  {1, 1, 3},     // CaCO3
  {6, 12, 6},    // C6H12O6
  {1, 3, 0},     // NH3
  {1, 1, 0},     // HCl
  {1, 1, 3},     // HNO3
  {1, 2, 2},     // Ca(OH)2
  {1, 1, 0}      // MgO
};

int mode = 0;   // 0 = main menu, 1 = element list, 2 = compound list, 9 = result
int page = 0;   // CHALLENGE 1: compound list page 0 (1-9) or page 1 (10-12)

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }
  display.setTextColor(SSD1306_WHITE);
  showMenu();
}

void loop() {
  char key = keypad.getKey();
  if (!key) return;

  if (key == '*') {                       // * = back to the main menu
    mode = 0;
    showMenu();
    return;
  }

  if (mode == 0) {
    if (key == 'A') { mode = 1; showMenu(); }
    if (key == 'B') { mode = 2; page = 0; showMenu(); }
  }
  else if (mode == 1 && key >= '0' && key <= '9') {
    showElement(key - '0');
  }
  else if (mode == 2 && key == '#') {     // # = the other page of compounds
    page = 1 - page;
    showMenu();
  }
  else if (mode == 2 && key >= '1' && key <= '9') {
    int c = page * 9 + (key - '1');       // page 1: key 1 = compound 9
    if (c < NUM_COMPOUNDS) showCompound(c);
  }
}

// Prints 35.5 as "35.5" and 18 as "18"
void printNumber(double v) {
  if (v == (long)v) display.print((long)v);
  else display.print(v, 1);
}

void showMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  if (mode == 0) {
    display.println("Chemistry Helper");
    display.println("");
    display.println("A: Element info");
    display.println("B: Molar mass");
  }
  else if (mode == 1) {
    display.println("Element (press 0-9):");
    display.println("0:H 1:C 2:N 3:O 4:Na");
    display.println("5:Mg 6:Al 7:S 8:Cl");
    display.println("9:Ca");
  }
  else if (mode == 2 && page == 0) {
    display.println("Compound (press 1-9):");
    display.println("1:H2O 2:CO2 3:NaCl");
    display.println("4:H2SO4 5:NaOH");
    display.println("6:CaCO3 7:C6H12O6");
    display.println("8:NH3 9:HCl  #=More");
  }
  else if (mode == 2) {
    display.println("Page 2 (press 1-3):");
    display.println("1:HNO3 2:Ca(OH)2");
    display.println("3:MgO");
    display.println("");
    display.println("#=Back to page 1");
  }
  display.println("");
  display.println("* = Menu");
  display.display();
}

void showElement(int i) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(elementSymbols[i]);
  display.setTextSize(1);
  display.println(elementNames[i]);
  display.print("Atomic number: ");
  display.println(atomicNumbers[i]);
  display.print("Atomic mass:   ");
  printNumber(atomicMasses[i]);
  display.println("");
  display.println("");
  display.println("0-9 = another, *=Menu");
  display.display();
}

void showCompound(int c) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(formulas[c]);
  display.print(" ");
  display.println(compoundNames[c]);

  double total = 0;
  for (int col = 0; col < 3; col++) {             // visit each column of row c
    int e = compoundElements[c][col];
    int n = compoundCounts[c][col];
    if (e == -1) continue;                        // empty place: skip it
    double part = n * atomicMasses[e];
    total += part;
    display.print(elementSymbols[e]);              // e.g.  "O: 4 x 16 = 64"
    display.print(": ");
    display.print(n);
    display.print(" x ");
    printNumber(atomicMasses[e]);
    display.print(" = ");
    printNumber(part);
    display.println("");
  }
  display.println("");
  display.print("Total: ");
  printNumber(total);
  display.println(" g/mol");
  display.display();
}
