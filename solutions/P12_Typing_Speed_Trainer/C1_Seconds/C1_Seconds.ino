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

// ---------- NEW: TIMING STATE ----------
String targetSequence = "";
String typedSequence = "";
unsigned long startTime = 0;
unsigned long endTime = 0;
bool testRunning = false;
bool testFinished = false;

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }
  display.setTextColor(SSD1306_WHITE);
  randomSeed(analogRead(34));
  showStartScreen();
}

void loop() {
  char key = keypad.getKey();
  if (!key) return;

  if (testFinished) {              // any key starts a new test
    startNewTest();
    return;
  }

  if (!testRunning) {              // waiting for # to start
    if (key == '#') {
      beginTest();
    }
    return;
  }

  // The test is running: record each digit typed
  if (key >= '0' && key <= '9') {
    typedSequence += key;
    showTypingScreen();
    if (typedSequence.length() == targetSequence.length()) {
      finishTest();
    }
  }
}

void generateTargetSequence() {
  targetSequence = "";
  for (int i = 0; i < 6; i++) {
    int digit = random(0, 10);
    targetSequence += String(digit);
  }
}

void showStartScreen() {
  generateTargetSequence();
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Typing Speed Test");
  display.println("");
  display.println("Sequence to type:");
  display.setTextSize(2);
  display.println(targetSequence);
  display.setTextSize(1);
  display.println("# = Start");
  display.display();
}

void beginTest() {
  typedSequence = "";
  testRunning = true;
  startTime = millis();            // the moment the test begins
  showTypingScreen();
}

void finishTest() {
  endTime = millis();              // the moment the test ends
  testRunning = false;
  testFinished = true;

  unsigned long elapsedMs = endTime - startTime;
  int correctCount = countCorrectDigits();

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Test Complete!");
  // CHALLENGE 1: seconds with 2 decimals. 1000.0 (not 1000) keeps the decimals!
  display.print("Time: ");
  display.print(elapsedMs / 1000.0, 2);
  display.println(" s");
  display.print("Accuracy: ");
  display.print(correctCount);
  display.print("/");
  display.println(targetSequence.length());
  display.println("");
  display.println("Any key = Retry");
  display.display();
}

int countCorrectDigits() {
  int correct = 0;
  for (int i = 0; i < targetSequence.length(); i++) {
    if (i < typedSequence.length() && typedSequence[i] == targetSequence[i]) {
      correct++;
    }
  }
  return correct;
}

void startNewTest() {
  testFinished = false;
  testRunning = false;
  showStartScreen();
}

void showTypingScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Target:");
  display.setTextSize(2);
  display.println(targetSequence);
  display.setTextSize(1);
  display.println("Your typing:");
  display.setTextSize(2);
  display.println(typedSequence);
  display.display();
}
