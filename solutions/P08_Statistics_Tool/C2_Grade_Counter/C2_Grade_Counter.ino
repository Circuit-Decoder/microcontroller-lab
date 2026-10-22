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

// ---------- NEW: STATISTICS STATE ----------
const int MAX_VALUES = 20;
double values[MAX_VALUES];    // the numbers you type are stored here
int count = 0;                // how many numbers are stored so far
String currentInput = "";
int stage = 1;                // 1 = typing numbers, 2 = results, 3 = sorted list

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

  if (key == '*') {                     // * = start a new data set
    count = 0;
    currentInput = "";
    stage = 1;
  }
  else if (stage == 1) {
    if (key >= '0' && key <= '9') {
      if (currentInput.length() < 7) currentInput += key;
    }
    else if (key == 'C') {              // C = decimal point (only once)
      if (currentInput.indexOf('.') == -1) currentInput += '.';
    }
    else if (key == 'D') {              // D = delete last character
      if (currentInput.length() > 0) {
        currentInput.remove(currentInput.length() - 1);
      }
    }
    else if (key == '#') {              // # = store this number
      if (currentInput.length() > 0 && currentInput != "." && count < MAX_VALUES) {
        values[count] = currentInput.toDouble();
        count++;
        currentInput = "";
      }
    }
    else if (key == 'A') {              // A = finished: calculate
      if (count > 0) {
        sortValues();
        stage = 2;
      }
    }
  }
  else if (key == 'B') {                // B = switch results / sorted list
    if (stage == 2) stage = 3;
    else stage = 2;
  }
  else if (key == 'C') {                // CHALLENGE 2: C = grade counter
    stage = 4;
  }
  showScreen();
}

// Bubble sort: compare neighbours, swap if they are in the wrong order
void sortValues() {
  for (int pass = 0; pass < count - 1; pass++) {
    for (int i = 0; i < count - 1 - pass; i++) {
      if (values[i] > values[i + 1]) {
        double temp = values[i];        // swap using a temporary box
        values[i] = values[i + 1];
        values[i + 1] = temp;
      }
    }
  }
}

double mean() {
  double sum = 0;
  for (int i = 0; i < count; i++) {
    sum += values[i];
  }
  return sum / count;
}

double median() {                       // the array must be sorted first!
  if (count % 2 == 1) {
    return values[count / 2];           // odd count: the middle value
  }
  // even count: the average of the two middle values
  return (values[count / 2 - 1] + values[count / 2]) / 2.0;
}

// Prints whole numbers without ".00", other numbers with 2 decimals
void printNumber(double v) {
  if (v == (long)v) display.print((long)v);
  else display.print(v, 2);
}

void showScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);

  if (stage == 1) {
    display.println("Statistics Tool");
    display.print("Stored: ");
    display.print(count);
    display.print("/");
    display.println(MAX_VALUES);
    display.setTextSize(2);
    display.println(currentInput);
    display.setTextSize(1);
    display.println("");
    display.println("#=Add  C:.  D:Del");
    display.println("A=Calculate  *=New");
  }
  else if (stage == 2) {
    display.print("Count:  ");
    display.println(count);
    display.print("Mean:   ");
    printNumber(mean());
    display.println("");
    display.print("Median: ");
    printNumber(median());
    display.println("");
    display.print("Lowest: ");
    printNumber(values[0]);
    display.println("");
    display.print("Highest:");
    printNumber(values[count - 1]);
    display.println("");
    display.print("Range:  ");
    printNumber(values[count - 1] - values[0]);
    display.println("");
    display.println("");
    display.println("B=List C=Grades *=New");
  }
  else if (stage == 4) {                // CHALLENGE 2: count the grades
    int lower[] = {80, 70, 60, 50, 40, 33, 0};
    String grade[] = {"A+", "A ", "A-", "B ", "C ", "D ", "F "};
    int done[7] = {0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < count; i++) {
      for (int g = 0; g < 7; g++) {     // the first grade whose lower mark fits
        if (values[i] >= lower[g]) { done[g]++; break; }
      }
    }
    for (int g = 0; g < 7; g++) {
      display.print(grade[g]);
      display.print(" (");
      display.print(lower[g]);
      display.print("+): ");
      display.println(done[g]);
    }
    display.print("B=Back");
  }
  else if (stage == 3) {
    display.println("Sorted (low-high):");
    for (int i = 0; i < count; i++) {
      printNumber(values[i]);
      display.print(" ");
    }
  }
  display.display();
}
