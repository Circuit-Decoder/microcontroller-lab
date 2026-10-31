// Project 10, Bonus 3 - Bar Chart
// C (on the results) draws one bar per value, scaled with map().
// Type up to 20 numbers, # after each (C = decimal point, D = delete).
// A = results (count, mean, median, lowest, highest, range).
// B = sorted list / results. * = start again.

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

const int MAX_VALUES = 20;
double values[MAX_VALUES];
int count = 0;
String typed = "";
int stage = 1;          // 1 = entering, 2 = results, 3 = sorted list

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

  if (key == '*') {
    count = 0;
    typed = "";
    stage = 1;
  } else if (stage == 1) {
    if (key >= '0' && key <= '9' && typed.length() < 7) {
      typed += key;
    } else if (key == 'C' && typed.indexOf('.') == -1) {
      typed += '.';
    } else if (key == 'D' && typed.length() > 0) {
      typed.remove(typed.length() - 1);
    } else if (key == '#' && hasDigit(typed) && count < MAX_VALUES) {
      values[count] = typed.toDouble();
      count++;
      typed = "";
    } else if (key == 'A' && count > 0) {
      sortValues();
      stage = 2;
    }
  } else if (key == 'B') {
    if (stage == 2) stage = 3;
    else stage = 2;
  } else if (key == 'C') {
    stage = 4;
  }
  showScreen();
}

bool hasDigit(String s) {
  for (int i = 0; i < s.length(); i++) {
    if (s[i] >= '0' && s[i] <= '9') return true;
  }
  return false;
}

void sortValues() {
  for (int pass = 0; pass < count - 1; pass++) {
    for (int i = 0; i < count - 1 - pass; i++) {
      if (values[i] > values[i + 1]) {
        double temp = values[i];
        values[i] = values[i + 1];
        values[i + 1] = temp;
      }
    }
  }
}

double mean() {
  double sum = 0;
  for (int i = 0; i < count; i++) sum += values[i];
  return sum / count;
}

double median() {
  if (count % 2 == 1) return values[count / 2];
  return (values[count / 2 - 1] + values[count / 2]) / 2.0;
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
    display.setCursor(0, 20);
    display.println(typed);
    display.setTextSize(1);
    display.setCursor(0, 48);
    display.println("#=add C:. D=del");
    display.println("A=results *=clear");
  } else if (stage == 2) {
    display.print("Results (");
    display.print(count);
    display.println(" values)");
    display.println("B=sorted C=chart");
    display.setCursor(0, 16);
    display.print("Mean:   ");
    display.println(mean(), 2);
    display.print("Median: ");
    display.println(median(), 2);
    display.print("Lowest: ");
    display.println(values[0], 2);
    display.print("Highest:");
    display.println(values[count - 1], 2);
    display.print("Range:  ");
    display.println(values[count - 1] - values[0], 2);
  } else if (stage == 4) {
    drawChart();
  } else {
    display.println("Sorted list");
    display.println("B = results");
    display.setCursor(0, 16);
    for (int i = 0; i < count; i++) {
      display.print(values[i], 1);
      display.print(" ");
    }
  }
  display.display();
}

// Bars in the blue area (y 16-63). The highest value is 48 px tall.
// Values are sorted, so the chart rises from left to right.
void drawChart() {
  display.println("Bar chart");
  display.println("B = results");
  int barWidth = 128 / count;            // 20 values: 6 px each
  double highest = values[count - 1];
  for (int i = 0; i < count; i++) {
    int h = 1;
    if (highest > 0) {
      h = map((long)(values[i] * 100), 0, (long)(highest * 100), 1, 48);
    }
    h = constrain(h, 1, 48);
    display.fillRect(i * barWidth, 64 - h, barWidth - 1, h,
                     SSD1306_WHITE);
  }
}
