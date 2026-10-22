// ---------- LIBRARIES ----------
// These lines load the ready-made code you installed in Step 3
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>

// ---------- OLED SETUP ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---------- KEYPAD SETUP ----------
const byte ROWS = 4;
const byte COLS = 4;

// The layout of the buttons on your keypad
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// ESP32 pins connected to the keypad's rows and columns
byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};

// Create the keypad using everything above
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String typed = "";              // CHALLENGE 3: every key pressed so far

void setup() {
  Serial.begin(115200);

  // Start the OLED. 0x3C is the display's address - keep it as it is.
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);   // stop here if the display is not working
  }

  display.clearDisplay();
  display.setTextSize(2);              // bigger text, easier to read
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Press a");
  display.println("key!");
  display.display();
}

void loop() {
  char key = keypad.getKey();   // check if a key is pressed

  if (key) {                    // if a key WAS pressed...
    Serial.println(key);        // also show it on the computer (for testing)

    display.clearDisplay();     // clear the old screen
    display.setCursor(0, 0);
    typed += key;               // add the new key to the end
    if (typed.length() > 40) typed = "";   // screen full: start again

    display.setTextSize(1);     // small label
    display.println("You typed:");
    display.setTextSize(2);     // size 2: 10 keys fit on one line
    display.println(typed);
    display.display();          // update the screen
  }
}
