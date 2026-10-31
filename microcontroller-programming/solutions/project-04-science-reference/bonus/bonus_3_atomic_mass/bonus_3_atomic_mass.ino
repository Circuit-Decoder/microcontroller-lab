// Project 04, Bonus 3 - Atomic Mass
// A third parallel array holds rounded atomic masses.
// A: physics constants on keys 0-9.  B: elements 1-20 by atomic number.
// In periodic-table mode, type digits, # = look up, * = clear.

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
  "1.6 x 10^-19 C", "8.31 J/mol.K", "6.02 x 10^23 /mol",
  "8.99 x 10^9 N.m^2/C^2", "1.38 x 10^-23 J/K", "9.11 x 10^-31 kg",
  "1.67 x 10^-27 kg"
};

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

int elementMasses[21] = {           // rounded, in u (check your book)
  0, 1, 4, 7, 9, 11,
  12, 14, 16, 19, 20,
  23, 24, 27, 28, 31,
  32, 35, 40, 39, 40
};

char mode = 'A';          // 'A' = constants, 'B' = periodic table
String typed = "";

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
  }
  display.setTextColor(SSD1306_WHITE);
  showHome();
}

void loop() {
  char key = keypad.getKey();
  if (!key) return;

  if (key == 'A' || key == 'B') {
    mode = key;
    typed = "";
    showHome();
  } else if (mode == 'A' && key >= '0' && key <= '9') {
    showConstant(key - '0');
  } else if (mode == 'B' && key >= '0' && key <= '9') {
    if (typed.length() < 3) {
      typed += key;
    }
    showTyping();
  } else if (mode == 'B' && key == '*') {
    typed = "";
    showTyping();
  } else if (mode == 'B' && key == '#') {
    lookUpElement(typed.toInt());
    typed = "";
  }
}

void showHome() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  if (mode == 'A') {
    display.println("Physics Constants");
    display.println("B = Periodic Table");
    display.setCursor(0, 24);
    display.println("Press 0-9 to view");
    display.println("a constant.");
  } else {
    display.println("Periodic Table 1-20");
    display.println("A = Constants");
    display.setCursor(0, 24);
    display.println("Type a number,");
    display.println("then press #.");
  }
  display.display();
}

void showConstant(int i) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(i);
  display.print(": ");
  display.println(constantNames[i]);
  display.print("Symbol: ");
  display.println(constantSymbols[i]);
  display.setCursor(0, 24);
  display.println(constantValues[i]);   // size 1: long values fit
  display.display();
}

void showTyping() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Periodic Table 1-20");
  display.println("Atomic number:");
  display.setTextSize(2);
  display.setCursor(0, 24);
  display.println(typed);
  display.display();
}

void lookUpElement(int n) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Atomic number ");
  display.println(n);
  display.setCursor(0, 24);
  if (n >= 1 && n <= 20) {
    display.setTextSize(2);
    display.println(elementNames[n]);
    display.setTextSize(1);
    display.print("Symbol: ");
    display.println(elementSymbols[n]);
    display.print("Mass: ");
    display.print(elementMasses[n]);
    display.println(" u");
  } else {
    display.println("Not in this device's");
    display.println("list (1-20 only).");
  }
  display.display();
}
