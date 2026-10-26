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

// ---------- NEW: NUMBER SYSTEM STATE ----------
String currentInput = "";
int mode = 0;   // 0 = menu, 1 = decimal in, 2 = binary in, 3 = key codes, 9 = result
                // CHALLENGE 1: 4 = hex in
bool shift = false;   // in hex mode, * turns on SHIFT for E, F, delete and menu
unsigned long resultValue = 0;

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

  if (mode == 4) {                      // CHALLENGE 1: hex has its own keys
    handleHexKey(key);
    return;
  }

  if (key == '*' || mode == 9) {        // * = menu; any key after a result too
    mode = 0;
    currentInput = "";
    showScreen();
    return;
  }

  if (mode == 0) {                      // choose a tool
    if (key == 'A') mode = 1;
    if (key == 'B') mode = 2;
    if (key == 'C') mode = 3;
    if (key == 'D') mode = 4;
    showScreen();
    return;
  }

  if (mode == 3) {                      // key codes: show the code of any key
    showKeyCode(key);
    return;
  }

  if (key == 'D') {                     // D = delete last digit
    if (currentInput.length() > 0) {
      currentInput.remove(currentInput.length() - 1);
    }
  }
  else if (key == '#') {                // # = convert
    if (currentInput.length() > 0) {
      if (mode == 1) resultValue = currentInput.toInt();
      if (mode == 2) resultValue = binaryToDecimal(currentInput);
      mode = 9;
    }
  }
  else if (mode == 1 && key >= '0' && key <= '9') {
    if (currentInput.length() < 9) currentInput += key;
  }
  else if (mode == 2 && (key == '0' || key == '1')) {   // binary: only 0 and 1
    if (currentInput.length() < 30) currentInput += key;
  }
  showScreen();
}

// ---------- CONVERSION FUNCTIONS ----------

// Converts n into any base from 2 to 16, by repeated division
String toBase(unsigned long n, int base) {
  const char digits[] = "0123456789ABCDEF";
  if (n == 0) return "0";
  String result = "";
  while (n > 0) {
    int remainder = n % base;                     // the next digit (from the right)
    result = String(digits[remainder]) + result;  // put it at the FRONT
    n = n / base;
  }
  return result;
}

// Converts a text of 0s and 1s into a number
unsigned long binaryToDecimal(String bits) {
  unsigned long value = 0;
  for (int i = 0; i < bits.length(); i++) {
    value = value * 2 + (bits[i] - '0');          // old bits move left, add the new bit
  }
  return value;
}

// ---------- SCREENS ----------

void showScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);

  if (mode == 0) {
    display.println("Number Systems");
    display.println("");
    display.println("A: Decimal -> others");
    display.println("B: Binary -> decimal");
    display.println("C: Key codes (ASCII)");
    display.println("D: Hex -> decimal");
  }
  else if (mode == 1 || mode == 2) {
    if (mode == 1) display.println("Decimal number:");
    if (mode == 2) display.println("Binary (0 and 1):");
    display.println(currentInput);
    display.println("");
    display.println("# = Convert  D = Del");
    display.println("* = Menu");
  }
  else if (mode == 4) {
    display.print("Hex number:");
    if (shift) display.print("  SHIFT");
    display.println("");
    display.setCursor(0, 16);   // row 16: below the yellow strip
    display.setTextSize(2);
    display.println(currentInput);
    display.setTextSize(1);
    display.setCursor(0, 40);
    display.println("A-D = A-D   # = Convert");
    display.println("*: SHIFT, then A=E B=F");
    display.print("   C=Delete D=Menu");
  }
  else if (mode == 3) {
    display.println("Key codes (ASCII)");
    display.println("");
    display.println("Press any key...");
    display.println("");
    display.println("* = Menu");
  }
  else if (mode == 9) {
    display.print("Dec: ");
    display.println(resultValue);
    display.print("Bin: ");
    display.println(toBase(resultValue, 2));
    display.print("Oct: ");
    display.println(toBase(resultValue, 8));
    display.print("Hex: ");
    display.println(toBase(resultValue, 16));
    display.println("");
    display.println("Any key = menu");
  }
  display.display();
}

void showKeyCode(char key) {
  int code = key;                       // a character IS a number!
  String bin = toBase(code, 2);
  while (bin.length() < 8) {
    bin = "0" + bin;                    // show all 8 bits
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Key: ");
  display.setTextSize(2);
  display.println(key);
  display.setTextSize(1);
  display.print("Dec: ");
  display.println(code);
  display.print("Bin: ");
  display.println(bin);
  display.print("Hex: ");
  display.println(toBase(code, 16));
  display.println("");
  display.println("Press a key, * = Menu");
  display.display();
}

// ---------- CHALLENGE 1: HEX TO DECIMAL ----------
// The keypad has A-D, but hex also needs E and F.
// Our answer: * works like the SHIFT key on a phone.
// After *, A gives E, B gives F, C deletes and D goes back to the menu.
void handleHexKey(char key) {
  if (key == '*') {
    shift = !shift;
  }
  else if (shift) {
    if (key == 'A') currentInput += 'E';
    if (key == 'B') currentInput += 'F';
    if (key == 'C' && currentInput.length() > 0) currentInput.remove(currentInput.length() - 1);
    if (key == 'D') { mode = 0; currentInput = ""; }
    shift = false;
  }
  else if (key == '#' && currentInput.length() > 0) {
    resultValue = hexToDecimal(currentInput);
    mode = 9;
  }
  else if ((key >= '0' && key <= '9') || (key >= 'A' && key <= 'D')) {
    if (currentInput.length() < 7) currentInput += key;
  }
  showScreen();
}

// Same idea as binaryToDecimal, but each digit is worth 16 times more
unsigned long hexToDecimal(String hex) {
  unsigned long value = 0;
  for (int i = 0; i < hex.length(); i++) {
    char h = hex[i];
    int digit;
    if (h >= '0' && h <= '9') digit = h - '0';        // '7' -> 7
    else digit = h - 'A' + 10;                         // 'A' -> 10 ... 'F' -> 15
    value = value * 16 + digit;
  }
  return value;
}
