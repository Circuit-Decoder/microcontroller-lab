#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include "MultiTap.h"        // our own keyboard file (in the second tab)

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

bool showingFinal = false;
// CHALLENGE 3: how fast can you type HELLO?
const String TARGET = "HELLO";
unsigned long startTime = 0;     // 0 = the clock has not started yet

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
  char key = keypad.getKey();      // 0 if no key is pressed
  if (key >= '0' && key <= '9' && startTime == 0) {
    startTime = millis();          // the clock starts at the first letter
  }

  if (mtUpdate(key)) {             // the keyboard did something
    showingFinal = false;
    showScreen();
  }

  if (key == '#') {                // # = finished
    mtLockIn();
    showingFinal = true;
    showFinalMessage();
  }
  else if (key == 'D') {           // D = clear everything
    mtClear();
    startTime = 0;
    showScreen();
  }
}

void showScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Type " + TARGET + ", then #");
  display.print(mtPreview());
  display.println("_");

  // Show the letters of the key being tapped, current one in [ ]
  display.setCursor(0, 48);
  if (mtActiveKey != 0) {
    String options = mtLetters[mtActiveKey - '0'];
    display.print(mtActiveKey);
    display.print(":");
    for (int i = 0; i < options.length(); i++) {
      if (i == mtTapCount) {
        display.print("[");
        display.print(options[i]);
        display.print("]");
      } else {
        display.print(" ");
        display.print(options[i]);
        display.print(" ");
      }
    }
  }
  display.setCursor(0, 56);
  display.print("#=Done *=Del D=Clear");
  display.display();
}

void showFinalMessage() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  unsigned long ms = millis() - startTime;
  display.println("You typed:");
  display.setCursor(0, 16);   // row 16: below the yellow strip
  display.setTextSize(2);
  display.println(mtText);
  display.setTextSize(1);
  if (mtText == TARGET) {
    display.print("Correct! Time: ");
    display.print(ms / 1000.0, 2);
    display.println(" s");
  } else {
    display.println("Not quite - try again");
  }
  display.println("D = new try");
  display.display();
}
