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

// ---------- NEW: LOOKUP ARRAYS ----------
// The three lists are lined up by position: place 0 in ALL
// three lists describes the SAME constant ("parallel arrays").
String constantNames[10] = {
  "Speed of Light", "Gravity (Earth)", "Planck's Const.",
  "Electron Charge", "Gas Constant", "Avogadro's No.",
  "Coulomb's Const.", "Boltzmann Const.", "Electron Mass", "Proton Mass"
};
String constantSymbols[10] = {
  "c", "g", "h", "e", "R", "N(A)", "k", "k(B)", "m(e)", "m(p)"
};
String constantValues[10] = {
  "3.0 x 10^8 m/s", "9.8 m/s^2", "6.63 x 10^-34 J.s",
  "1.6 x 10^-19 C", "8.31 J/mol.K", "6.02 x 10^23",
  "8.99 x 10^9", "1.38 x 10^-23", "9.11 x 10^-31 kg", "1.67 x 10^-27 kg"
};

// ---------- NEW: PERIODIC TABLE DATA ----------
// Place 0 is empty ("") because no element has atomic number 0.
// Same idea as Part A: the two lists are lined up by position.
String elementNames[21] = {
  "", "Hydrogen", "Helium", "Lithium", "Beryllium", "Boron",
  "Carbon", "Nitrogen", "Oxygen", "Fluorine", "Neon",
  "Sodium", "Magnesium", "Aluminium", "Silicon", "Phosphorus",
  "Sulfur", "Chlorine", "Argon", "Potassium", "Calcium"
};
String elementSymbols[21] = {
  "", "H", "He", "Li", "Be", "B",
  "C", "N", "O", "F", "Ne",
  "Na", "Mg", "Al", "Si", "P",
  "S", "Cl", "Ar", "K", "Ca"
};

String enteredNumber = "";   // the digits typed so far

// CHALLENGE 4: which tool is running? 0 = menu, 1 = constants, 2 = periodic table
int tool = 0;

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

  if (key == 'D') {                 // D = back to the menu, from any tool
    tool = 0;
    enteredNumber = "";
    showMenu();
    return;
  }

  if (tool == 0) {                  // ----- menu -----
    if (key == 'A') { tool = 1; showWelcomeScreen(); }
    if (key == 'B') { tool = 2; showEntryScreen(); }
  }
  else if (tool == 1) {             // ----- physics constants -----
    if (key >= '0' && key <= '9') showConstant(key - '0');
  }
  else if (tool == 2) {             // ----- periodic table -----
    if (key == '#') lookupElement();
    else if (key == '*') { enteredNumber = ""; showEntryScreen(); }
    else if (key >= '0' && key <= '9') { enteredNumber += key; showEntryScreen(); }
  }
}

void showMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("SCIENCE REFERENCE");
  display.println("");
  display.println("A: Physics constants");
  display.println("B: Periodic table");
  display.println("");
  display.println("D: back to this menu");
  display.display();
}

void showWelcomeScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Physics Constants");
  display.println("");
  display.println("Press 0-9 to view");
  display.println("a constant.");
  display.display();
}

void showConstant(int index) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(constantNames[index]);
  display.print("Symbol: ");
  display.println(constantSymbols[index]);
  display.println("");
  display.setTextSize(2);
  display.println(constantValues[index]);
  display.display();
}

void showEntryScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Enter atomic #:");
  display.setCursor(0, 16);   // row 16: below the yellow strip
  display.setTextSize(2);
  display.println(enteredNumber);
  display.setTextSize(1);
  display.println("# = Look up * = Clear");
  display.display();
}

void lookupElement() {
  int number = enteredNumber.toInt();   // text "17" becomes number 17

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);

  if (number >= 1 && number <= 20) {
    display.print("Atomic #: ");
    display.println(number);
    display.setCursor(0, 16);   // row 16: below the yellow strip
    display.setTextSize(2);
    display.println(elementNames[number]);
    display.setTextSize(1);
    display.print("Symbol: ");
    display.println(elementSymbols[number]);
  } else {
    display.println("Not in this");
    display.println("device's list");
    display.println("(only 1-20 stored)");
  }
  display.display();
  enteredNumber = "";   // reset for the next lookup
}
