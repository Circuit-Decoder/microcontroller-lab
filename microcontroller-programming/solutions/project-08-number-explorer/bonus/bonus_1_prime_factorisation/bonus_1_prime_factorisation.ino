// Project 08, Bonus 1 - Prime factorisation
// # breaks the number into primes: 360 = 2x2x2x3x3x5

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

String typed = "";
unsigned long num1 = 0;
int stage = 1;     // 1 = first number, 2 = second number (GCD), 3 = result

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
  }
  display.setTextColor(SSD1306_WHITE);
  showEntry();
}

void loop() {
  char key = keypad.getKey();
  if (!key) return;

  if (key == '*' || stage == 3) {
    typed = "";
    stage = 1;
    showEntry();
    return;
  }
  if (key >= '0' && key <= '9') {
    if (typed.length() < 9) typed += key;
    showEntry();
    return;
  }
  if (typed.length() == 0) return;          // type a number first

  unsigned long n = typed.toInt();
  if (stage == 1) {
    if (key == 'A') { showPrime(n); stage = 3; }
    else if (key == 'B') { showFactors(n); stage = 3; }
    else if (key == 'C') { num1 = n; typed = ""; stage = 2; showEntry(); }
    else if (key == 'D') { showDigits(n); stage = 3; }
    else if (key == '#') { showPrimeFactors(n); stage = 3; }
  } else if (stage == 2 && key == '#') {
    showGcdLcm(num1, n);
    stage = 3;
  }
}

unsigned long firstFactor(unsigned long n) {
  unsigned long d = 2;
  while (d * d <= n) {
    if (n % d == 0) return d;
    d++;
  }
  return 0;
}

unsigned long gcd(unsigned long a, unsigned long b) {
  while (b != 0) {
    unsigned long r = a % b;
    a = b;
    b = r;
  }
  return a;
}

void startScreen(String title) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(title);
  display.println("Any key = new number");
  display.setCursor(0, 16);
}

void showEntry() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  if (stage == 1) {
    display.println("Number Explorer");
    display.println("A:Prime B:Factors");
  } else {
    display.print("GCD/LCM with ");
    display.println(num1);
    display.println("2nd number, then #");
  }
  display.setTextSize(2);
  display.setCursor(0, 24);
  display.println(typed);
  if (stage == 1) {
    display.setTextSize(1);
    display.setCursor(0, 56);
    display.println("C:GCD D:Dig #:Prm");
  }
  display.display();
}

void showPrime(unsigned long n) {
  startScreen("Prime check");
  display.println(n);
  if (n < 2) {
    display.println("is NOT prime.");
    display.println("Primes start at 2.");
  } else {
    unsigned long f = firstFactor(n);
    if (f == 0) {
      display.println("is PRIME!");
    } else {
      display.println("is NOT prime:");
      display.print(f);
      display.print(" x ");
      display.println(n / f);
    }
  }
  display.display();
}

void showFactors(unsigned long n) {
  startScreen("Factors");
  if (n == 0 || n > 100000) {
    display.println("Works for 1-100000.");
    display.display();
    return;
  }
  int count = 0;
  for (unsigned long d = 1; d <= n; d++) {
    if (n % d == 0) {
      display.print(d);
      display.print(" ");
      count++;
    }
  }
  display.println();
  display.print("(");
  display.print(count);
  display.println(" factors)");
  display.display();
}

void showGcdLcm(unsigned long a, unsigned long b) {
  startScreen("GCD and LCM");
  display.print(a);
  display.print(" and ");
  display.println(b);
  if (a == 0 || b == 0) {
    display.println("Use numbers above 0");
  } else {
    unsigned long g = gcd(a, b);
    unsigned long long lcm = (unsigned long long)(a / g) * b;
    display.print("GCD = ");
    display.println(g);
    display.print("LCM = ");
    display.println(lcm);
  }
  display.display();
}

void showDigits(unsigned long n) {
  int digitCount = 0;
  int digitSum = 0;
  unsigned long temp = n;
  while (temp > 0) {
    digitSum += temp % 10;
    temp = temp / 10;
    digitCount++;
  }
  if (n == 0) digitCount = 1;

  startScreen("Even/odd and digits");
  display.print(n);
  if (n % 2 == 0) display.println(" is EVEN");
  else display.println(" is ODD");
  display.print("Digits: ");
  display.println(digitCount);
  display.print("Digit sum: ");
  display.println(digitSum);
  display.print("Divisible by 3? ");
  if (digitSum % 3 == 0) display.println("YES");
  else display.println("NO");
  display.display();
}

void showPrimeFactors(unsigned long n) {
  startScreen("Prime factors");
  if (n < 2) {
    display.println("Use 2 or more.");
    display.display();
    return;
  }
  display.print(n);
  display.println(" =");
  bool first = true;
  while (n > 1) {
    unsigned long f = firstFactor(n);
    if (f == 0) f = n;                 // what is left is prime
    if (!first) display.print("x");
    display.print(f);
    first = false;
    n = n / f;                         // divide it out and go again
  }
  display.display();
}
