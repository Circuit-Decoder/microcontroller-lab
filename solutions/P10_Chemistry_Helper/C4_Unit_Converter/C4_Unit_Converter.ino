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

// ---------- CHALLENGE 4: UNIT CONVERTER ----------
// The same "lookup + formula" idea as the Chemistry Helper:
// parallel arrays hold the names and the multiply factors.
const int NUM_CONVERSIONS = 6;
String fromUnit[NUM_CONVERSIONS] = {"km", "miles", "kg", "pounds", "C", "F"};
String toUnit[NUM_CONVERSIONS]   = {"miles", "km", "pounds", "kg", "F", "C"};
double factor[NUM_CONVERSIONS]   = {0.621, 1 / 0.621, 2.205, 1 / 2.205, 0, 0};
// (temperatures need their own formula, so their factor is not used)

int choice = -1;          // -1 = menu, 0..5 = typing a value
String currentInput = "";

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

  if (key == '*') {                         // * = back to the menu
    choice = -1;
    currentInput = "";
    showMenu();
    return;
  }
  if (choice == -1) {                       // menu: keys 1-6 pick a conversion
    if (key >= '1' && key <= '6') {
      choice = key - '1';
      showInput();
    }
    return;
  }
  if (key >= '0' && key <= '9' && currentInput.length() < 8) currentInput += key;
  else if (key == 'B' && currentInput.length() == 0) currentInput = "-";      // minus
  else if (key == 'C' && currentInput.indexOf('.') == -1) currentInput += '.'; // decimal
  else if (key == 'D' && currentInput.length() > 0) currentInput.remove(currentInput.length() - 1);
  else if (key == '#' && currentInput.length() > 0) {
    showAnswer(currentInput.toDouble());
    currentInput = "";
    return;
  }
  showInput();
}

double convert(int c, double value) {
  if (c == 4) return value * 9 / 5 + 32;     // Celsius -> Fahrenheit
  if (c == 5) return (value - 32) * 5 / 9;   // Fahrenheit -> Celsius
  return value * factor[c];                  // everything else: multiply
}

void showMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Unit Converter");
  for (int i = 0; i < NUM_CONVERSIONS; i++) {
    display.print(i + 1);
    display.print(": ");
    display.print(fromUnit[i]);
    display.print(" -> ");
    display.println(toUnit[i]);
  }
  display.display();
}

void showInput() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(fromUnit[choice]);
  display.print(" -> ");
  display.println(toUnit[choice]);
  display.print("Value in ");
  display.print(fromUnit[choice]);
  display.println(":");
  display.setTextSize(2);
  display.println(currentInput);
  display.setTextSize(1);
  display.setCursor(0, 48);
  display.println("B:-  C:.  D:Del  #=OK");
  display.print("* = Menu");
  display.display();
}

void showAnswer(double value) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(value, 2);
  display.print(" ");
  display.print(fromUnit[choice]);
  display.println(" =");
  display.setCursor(0, 16);   // row 16: below the yellow strip
  display.setTextSize(2);
  display.println(convert(choice, value), 2);
  display.setTextSize(1);
  display.println(toUnit[choice]);
  display.setCursor(0, 48);
  display.println("Type another value");
  display.print("* = Menu");
  display.display();
}
