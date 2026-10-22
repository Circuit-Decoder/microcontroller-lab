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

// ---------- NEW: NUMBER EXPLORER STATE ----------
String currentInput = "";
unsigned long num1 = 0;
unsigned long num2 = 0;
int stage = 1;   // 1 = typing a number, 2 = typing 2nd number (GCD), 3 = result

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

  if (key == '*' || stage == 3) {      // * clears; any key after a result too
    resetExplorer();
    return;
  }

  if (key >= '0' && key <= '9') {       // a digit: add it (max 9 digits)
    if (currentInput.length() < 9) {
      currentInput += key;
    }
    showScreen();
    return;
  }

  if (currentInput.length() == 0) return;   // type a number first!

  if (stage == 1) {
    num1 = currentInput.toInt();
    if (key == 'A') {
      showPrime(num1);
      stage = 3;
    }
    else if (key == 'B') {
      showFactors(num1);
      stage = 3;
    }
    else if (key == 'C') {             // GCD needs a second number
      currentInput = "";
      stage = 2;
      showScreen();
    }
    else if (key == 'D') {
      showEvenOdd(num1);
      stage = 3;
    }
  }
  else if (stage == 2 && key == '#') {
    num2 = currentInput.toInt();
    showGcdLcm(num1, num2);
    stage = 3;
  }
}

// ---------- MATHS FUNCTIONS ----------

// Returns the smallest factor of n (bigger than 1),
// or 0 if there is none (which means n is prime).
unsigned long smallestFactor(unsigned long n) {
  unsigned long d = 2;
  unsigned long found = 0;
  while (d * d <= n) {
    if (n % d == 0) {          // remainder 0: d divides n exactly
      found = d;
      break;                   // found one: stop the loop early
    }
    d++;
  }
  return found;
}

// Greatest Common Divisor, using Euclid's method
unsigned long gcd(unsigned long a, unsigned long b) {
  while (b != 0) {
    unsigned long r = a % b;
    a = b;
    b = r;
  }
  return a;
}

// ---------- SCREENS ----------

void showScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  if (stage == 1) {
    display.println("Number Explorer");
    display.println("Number:");
    display.setTextSize(2);
    display.println(currentInput);
    display.setTextSize(1);
    display.println("");
    display.println("A:Prime  B:Factors");
    display.println("C:GCD/LCM D:Even/Odd");
  }
  else if (stage == 2) {
    display.println("GCD / LCM");
    display.print("1st: ");
    display.println(num1);
    display.println("2nd number:");
    display.setTextSize(2);
    display.println(currentInput);
    display.setTextSize(1);
    display.println("# = Calculate");
  }
  display.display();
}

void startResult() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
}

void showPrime(unsigned long n) {
  startResult();
  display.println("Prime check:");
  display.setTextSize(2);
  display.println(n);
  display.setTextSize(1);
  if (n < 2) {
    display.println("is NOT prime.");
    display.println("(Primes start at 2)");
  } else {
    unsigned long f = smallestFactor(n);
    if (f == 0) {
      display.println("is PRIME!");
    } else {
      display.println("is NOT prime.");
      display.print("= ");
      display.print(f);
      display.print(" x ");
      display.println(n / f);
    }
  }
  display.println("");
  display.println("Any key = new number");
  display.display();
}

void showFactors(unsigned long n) {
  startResult();
  if (n == 0) {                     // CHALLENGE 3: no upper limit needed now
    display.println("Factors tool works");
    display.println("for numbers above 0.");
    display.println("");
    display.println("Any key = new number");
    display.display();
    return;
  }
  // CHALLENGE 3: only go up to the square root. Each time d divides n,
  // we get TWO factors: d (small) and n / d (big).
  String small = "";
  String big = "";
  int count = 0;
  for (unsigned long d = 1; d * d <= n; d++) {
    if (n % d == 0) {
      small += String(d) + " ";
      count++;
      if (d != n / d) {                     // 6 x 6 = 36: count 6 only once
        big = String(n / d) + " " + big;    // put at the FRONT, so it stays in order
        count++;
      }
    }
  }
  String list = small + big;
  display.print(n);
  display.print(" has ");
  display.print(count);
  display.println(" factors:");
  display.println(list);
  display.display();
}

void showGcdLcm(unsigned long a, unsigned long b) {
  startResult();
  if (a == 0 || b == 0) {
    display.println("Use numbers above 0");
  } else {
    unsigned long g = gcd(a, b);
    unsigned long long lcm = (unsigned long long)(a / g) * b;
    display.print(a);
    display.print(" and ");
    display.println(b);
    display.println("");
    display.print("GCD = ");
    display.println(g);
    display.print("LCM = ");
    display.println(lcm);
  }
  display.println("");
  display.println("Any key = new number");
  display.display();
}

void showEvenOdd(unsigned long n) {
  int digitSum = 0;
  int digitCount = 0;
  unsigned long temp = n;
  while (temp > 0) {
    digitSum += temp % 10;     // take the last digit
    temp = temp / 10;          // remove the last digit
    digitCount++;
  }
  if (n == 0) digitCount = 1;

  startResult();
  display.setTextSize(2);
  display.println(n);
  display.setTextSize(1);
  if (n % 2 == 0) {
    display.println("is EVEN");
  } else {
    display.println("is ODD");
  }
  display.print("Digits: ");
  display.println(digitCount);
  display.print("Digit sum: ");
  display.println(digitSum);
  display.print("Divisible by 3? ");
  display.println(n % 3 == 0 ? "YES" : "NO");
  display.println("Any key = new number");
  display.display();
}

void resetExplorer() {
  currentInput = "";
  num1 = 0;
  num2 = 0;
  stage = 1;
  showScreen();
}
