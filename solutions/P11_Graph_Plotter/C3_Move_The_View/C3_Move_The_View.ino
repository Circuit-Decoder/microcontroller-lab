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

// ---------- NEW: GRAPH PLOTTER STATE ----------
// Three kinds of graph. Each needs 2 or 3 numbers (coefficients).
String formulaText[3] = {"y = mx + c", "y = ax^2 + bx + c", "y = a sin(bx)"};
String coefNames[3][3] = {{"m", "c", ""}, {"a", "b", "c"}, {"a", "b", ""}};
int coefCount[3] = {2, 3, 2};

int graphType = 0;        // 0 = line, 1 = quadratic, 2 = sine
double coef[3];           // the numbers the user types
int k = 0;                // which coefficient we are typing now
String currentInput = "";
int mode = 0;             // 0 = menu, 1 = typing numbers, 2 = showing graph
double scale = 6;         // pixels per 1 unit (zoom level)
double viewX = 0;         // CHALLENGE 3: the maths point shown in the centre
double viewY = 0;

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

  if (key == '*') {                         // * = back to the menu
    mode = 0;
    showScreen();
    return;
  }

  if (mode == 0) {                          // choose a graph type
    if (key == 'A' || key == 'B' || key == 'C') {
      graphType = key - 'A';                // A -> 0, B -> 1, C -> 2
      k = 0;
      currentInput = "";
      mode = 1;
    }
  }
  else if (mode == 1) {                     // typing the numbers
    if (key >= '0' && key <= '9') {
      if (currentInput.length() < 6) currentInput += key;
    }
    else if (key == 'B') {                  // minus sign, only at the start
      if (currentInput.length() == 0) currentInput = "-";
    }
    else if (key == 'C') {                  // decimal point, only once
      if (currentInput.indexOf('.') == -1) currentInput += '.';
    }
    else if (key == 'D') {                  // delete
      if (currentInput.length() > 0) {
        currentInput.remove(currentInput.length() - 1);
      }
    }
    else if (key == '#') {                  // this number is finished
      coef[k] = currentInput.toDouble();
      currentInput = "";
      k++;
      if (k == coefCount[graphType]) {      // all numbers typed: draw!
        scale = 6;
        viewX = 0;
        viewY = 0;
        mode = 2;
      }
    }
  }
  else if (mode == 2) {                     // zoom
    if (key == 'A' && scale < 48) scale = scale * 2;     // zoom in
    if (key == 'B' && scale > 1.5) scale = scale / 2;    // zoom out
    // CHALLENGE 3: move the view by 16 pixels
    if (key == '4') viewX -= 16 / scale;     // left
    if (key == '6') viewX += 16 / scale;     // right
    if (key == '2') viewY += 16 / scale;     // up
    if (key == '8') viewY -= 16 / scale;     // down
  }
  showScreen();
}

// The maths: y for any x
double f(double x) {
  if (graphType == 0) return coef[0] * x + coef[1];
  if (graphType == 1) return coef[0] * x * x + coef[1] * x + coef[2];
  return coef[0] * sin(coef[1] * x);
}

// Maths coordinates -> screen pixels.  (0,0) is in the centre.
int toScreenX(double x) {
  double px = 64 + (x - viewX) * scale;     // CHALLENGE 3: measured from the view
  if (px < -1000) px = -1000;
  if (px > 1000) px = 1000;
  return round(px);
}
int toScreenY(double y) {
  double py = 32 - (y - viewY) * scale;     // screen y grows DOWNWARDS
  if (py < -1000) py = -1000;               // keep huge values in a safe range
  if (py > 1000) py = 1000;
  return round(py);
}

void drawGraph() {
  display.clearDisplay();

  // axes
  // the axes are where x = 0 and y = 0 are NOW (they move with the view)
  display.drawLine(0, toScreenY(0), 127, toScreenY(0), SSD1306_WHITE);   // x-axis
  display.drawLine(toScreenX(0), 0, toScreenX(0), 63, SSD1306_WHITE);    // y-axis

  // a small tick mark at every whole number, if they are not too close
  if (scale >= 4) {
    for (int i = -20; i <= 20; i++) {
      display.drawPixel(toScreenX(i), toScreenY(0) - 1, SSD1306_WHITE);
      display.drawPixel(toScreenX(0) + 1, toScreenY(i), SSD1306_WHITE);
    }
  }

  // the curve: one point for every column of pixels, joined by lines
  int prevX = 0;
  int prevY = toScreenY(f((0 - 64) / scale + viewX));
  for (int px = 1; px < 128; px++) {
    double x = (px - 64) / scale + viewX;   // screen column -> maths x
    int py = toScreenY(f(x));
    display.drawLine(prevX, prevY, px, py, SSD1306_WHITE);
    prevX = px;
    prevY = py;
  }

  // label
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(formulaText[graphType]);
  display.display();
}

void showScreen() {
  if (mode == 2) {
    drawGraph();
    return;
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  if (mode == 0) {
    display.println("Graph Plotter");
    display.println("");
    display.println("A: y = mx + c");
    display.println("B: y = ax^2 + bx + c");
    display.println("C: y = a sin(bx)");
  }
  else if (mode == 1) {
    display.println(formulaText[graphType]);
    display.println("");
    display.print(coefNames[graphType][k]);
    display.print(" = ");
    display.setTextSize(2);
    display.println(currentInput);
    display.setTextSize(1);
    display.println("");
    display.println("B:-  C:.  D:Del");
    display.println("# = OK   * = Menu");
  }
  display.display();
}
