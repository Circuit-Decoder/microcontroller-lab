// Project 11, Bonus 2 - Binary Addition
// D = type two binary numbers (# after each) and see the sum in binary.

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

// 0 = menu, 1 = decimal in, 2 = binary in, 3 = key codes, 4 = result
int stage = 0;
String typed = "";
unsigned long result = 0;
char lastKey = 0;
String firstBits = "";  // stage 5 = first number, 6 = second, 7 = sum

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

  if (key == '*' || stage == 4 || stage == 7) {   // back to the menu
    stage = 0;
    typed = "";
    lastKey = 0;
  } else if (stage == 0) {
    if (key == 'A') stage = 1;
    if (key == 'B') stage = 2;
    if (key == 'C') stage = 3;
    if (key == 'D') stage = 5;
  } else if ((stage == 5 || stage == 6) && key == '#') {
    if (typed.length() > 0) {
      if (stage == 5) {
        firstBits = typed;
        typed = "";
        stage = 6;
      } else {
        stage = 7;                         // typed = second number
      }
    }
  } else if ((stage == 5 || stage == 6) && (key == '0' || key == '1')) {
    if (typed.length() < 16) typed += key;
  } else if (stage == 3) {
    lastKey = key;
  } else if (key == 'D' && typed.length() > 0) {
    typed.remove(typed.length() - 1);
  } else if (key == '#' && typed.length() > 0) {
    if (stage == 1) result = typed.toInt();
    else result = binaryToDecimal(typed);
    stage = 4;
  } else if (stage == 1 && key >= '0' && key <= '9') {
    if (typed.length() < 9) typed += key;
  } else if (stage == 2 && (key == '0' || key == '1')) {
    if (typed.length() < 30) typed += key;
  }
  showScreen();
}

// Converts n into any base from 2 to 16, by repeated division
String toBase(unsigned long n, int base) {
  const char digits[] = "0123456789ABCDEF";
  if (n == 0) return "0";
  String text = "";
  while (n > 0) {
    text = String(digits[n % base]) + text;   // the new digit goes in front
    n = n / base;
  }
  return text;
}

unsigned long binaryToDecimal(String bits) {
  unsigned long value = 0;
  for (int i = 0; i < bits.length(); i++) {
    value = value * 2 + (bits[i] - '0');
  }
  return value;
}

void showScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  if (stage == 0) {
    display.println("Number Systems");
    display.setCursor(0, 24);
    display.println("A: Decimal -> others");
    display.println("B: Binary -> others");
    display.println("C: Key codes (ASCII)");
    display.println("D: Binary addition");
  } else if (stage == 5 || stage == 6) {
    display.println("Binary addition");
    display.println("0/1, # = next, D=del");
    display.setCursor(0, 24);
    if (stage == 6) {
      display.print("  ");
      display.println(firstBits);
      display.print("+ ");
    } else {
      display.print("  ");
    }
    display.println(typed);
  } else if (stage == 7) {
    showSum();
  } else if (stage == 1 || stage == 2) {
    if (stage == 1) display.println("Decimal number:");
    else display.println("Binary (0 and 1):");
    display.setCursor(0, 16);
    display.println(typed);
    display.setCursor(0, 48);
    display.println("#=convert D=delete");
    display.println("*=menu");
  } else if (stage == 3) {
    display.println("Key codes (ASCII)");
    display.println("Press a key, *=menu");
    if (lastKey != 0) {
      String bits = toBase(lastKey, 2);
      while (bits.length() < 8) bits = "0" + bits;   // a full byte
      display.setCursor(0, 24);
      display.print("Key: ");
      display.println(lastKey);
      display.print("Dec: ");
      display.println((int)lastKey);
      display.print("Bin: ");
      display.println(bits);
      display.print("Hex: ");
      display.println(toBase(lastKey, 16));
    }
  } else {
    display.println("Result");
    display.println("Any key = menu");
    display.print("Dec: ");
    display.println(result);
    display.print("Oct: ");
    display.println(toBase(result, 8));
    display.print("Hex: ");
    display.println(toBase(result, 16));
    display.println("Bin:");
    display.println(toBase(result, 2));
  }
  display.display();
}

// Column addition, right to left, exactly as on paper
String addBinary(String a, String b) {
  String sum = "";
  int carry = 0;
  int i = a.length() - 1;
  int j = b.length() - 1;
  while (i >= 0 || j >= 0 || carry > 0) {
    int total = carry;
    if (i >= 0) total += a[i--] - '0';
    if (j >= 0) total += b[j--] - '0';
    sum = String(total % 2) + sum;    // the bit to write down
    carry = total / 2;                // the bit to carry
  }
  return sum;
}

void showSum() {
  String sum = addBinary(firstBits, typed);
  int width = sum.length();
  display.println("Binary addition");
  display.println("Any key = menu");
  display.setCursor(0, 24);
  display.print("  ");
  printRight(firstBits, width);
  display.print("+ ");
  printRight(typed, width);
  display.println("  -----------------");
  display.print("= ");
  printRight(sum, width);
  display.print("(");
  display.print(binaryToDecimal(firstBits));
  display.print(" + ");
  display.print(binaryToDecimal(typed));
  display.print(" = ");
  display.print(binaryToDecimal(sum));
  display.println(")");
}

// prints bits right-aligned in a column of the given width
void printRight(String bits, int width) {
  for (int k = bits.length(); k < width; k++) display.print(' ');
  display.println(bits);
}
