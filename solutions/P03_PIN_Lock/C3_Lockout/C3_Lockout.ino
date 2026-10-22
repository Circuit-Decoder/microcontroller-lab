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

// ---------- NEW: PIN SETUP ----------
String correctPIN = "1234";   // <-- change this to your own secret PIN
String enteredPIN = "";       // what the user types, starting empty
int wrongTries = 0;           // CHALLENGE 3: wrong PINs in a row

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }
  display.setTextColor(SSD1306_WHITE);
  showEnterPINScreen();
}

void loop() {
  char key = keypad.getKey();

  if (key) {                       // a key was pressed
    if (key == '#') {              // '#' means "check the PIN now"
      checkPIN();
    }
    else if (key == '*') {         // '*' means "clear and start over"
      enteredPIN = "";
      showEnterPINScreen();
    }
    else {                         // any other key is added to the PIN
      enteredPIN += key;
      showEnterPINScreen();
    }
  }
}

// ---------- HELPER FUNCTIONS ----------
// A function is a named block of code we can reuse.
// We write it once, then "call" it by its name.

void showEnterPINScreen() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Enter PIN:");
  display.println(enteredPIN);
  display.setTextSize(1);
  display.println("");
  display.println("# = Confirm * = Clear");
  display.display();
}

void checkPIN() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);

  if (enteredPIN == correctPIN) {
    display.println("Access");
    display.println("Granted!");
    wrongTries = 0;                 // correct: start counting again
  } else {
    display.println("Access");
    display.println("Denied!");
    wrongTries++;
  }
  display.display();

  if (wrongTries >= 3) {            // 3 wrong in a row: lock for 10 seconds
    delay(2000);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Locked!");
    display.println("Wait 10 s");
    display.display();
    delay(10000);                   // keys are ignored while we wait
    wrongTries = 0;
  }

  delay(2000);          // show the result for 2 seconds
  enteredPIN = "";      // reset for the next try
  showEnterPINScreen();
}
