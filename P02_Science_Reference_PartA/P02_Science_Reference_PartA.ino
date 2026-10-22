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
 
void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }
  display.setTextColor(SSD1306_WHITE);
  showWelcomeScreen();
}
 
void loop() {
  char key = keypad.getKey();
  if (!key) return;
 
  // Only respond to number keys 0-9
  if (key >= '0' && key <= '9') {
    int index = key - '0';   // turns the character '5' into the number 5
    showConstant(index);
  }
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
