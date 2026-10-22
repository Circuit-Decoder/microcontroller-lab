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

// CHALLENGE 3: a third parallel array - atomic mass (rounded, like your textbook)
double atomicMasses[21] = {
  0, 1, 4, 7, 9, 11,
  12, 14, 16, 19, 20,
  23, 24, 27, 28, 31,
  32, 35.5, 40, 39, 40
};

String enteredNumber = "";   // the digits typed so far

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }
  display.setTextColor(SSD1306_WHITE);
  showEntryScreen();
}

void loop() {
  char key = keypad.getKey();
  if (!key) return;

  if (key == '#') {                         // finished typing: look it up
    lookupElement();
  }
  else if (key == '*') {                    // clear and start again
    enteredNumber = "";
    showEntryScreen();
  }
  else if (key >= '0' && key <= '9') {      // a digit: add it to the end
    enteredNumber += key;
    showEntryScreen();
  }
  // Letter keys (A-D) are ignored in this project
}

void showEntryScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Enter atomic #:");
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
    display.setTextSize(2);
    display.println(elementNames[number]);
    display.setTextSize(1);
    display.print("Symbol: ");
    display.println(elementSymbols[number]);
    display.print("Mass: ");
    display.println(atomicMasses[number], 1);   // 1 decimal place
  } else {
    display.println("Not in this");
    display.println("device's list");
    display.println("(only 1-20 stored)");
  }
  display.display();
  enteredNumber = "";   // reset for the next lookup
}
