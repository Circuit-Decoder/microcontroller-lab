#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include "BluetoothKeys.h"      // NEW: our Bluetooth keyboard tab
 
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
 
// The name the computer will see in its Bluetooth list
const char* REMOTE_NAME = "CD Slide Remote";
 
int slide = 1;                  // which slide we think is showing
bool showRunning = false;
unsigned long showStart = 0;    // when the slideshow started
bool typingNumber = false;      // C = go to a slide number
String slideNumber = "";
bool wasConnected = false;
unsigned long lastDraw = 0;
 
void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }
  display.setTextColor(SSD1306_WHITE);
  btBegin(REMOTE_NAME);         // start Bluetooth: "I am a keyboard!"
  showScreen();
}
 
void loop() {
  bool connected = btConnected();
  if (connected != wasConnected) {          // connected or disconnected just now
    wasConnected = connected;
    showScreen();
  }
 
  char key = keypad.getKey();
  if (key && connected) {
    if (typingNumber) handleNumberKey(key);
    else handleKey(key);
    showScreen();
  }
 
  if (showRunning && millis() - lastDraw >= 1000) {   // update the timer
    showScreen();
  }
}
 
// ---------- Normal keys ----------
void handleKey(char key) {
  if (key == '#') {                          // next slide
    btPress(KEY_RIGHT);
    slide++;
  }
  else if (key == '*') {                     // previous slide
    btPress(KEY_LEFT);
    if (slide > 1) slide--;
  }
  else if (key == 'A') {                     // start the slideshow (F5)
    btPress(KEY_F5_KEY);
    slide = 1;
    showRunning = true;
    showStart = millis();
  }
  else if (key == 'B') {                     // black screen on / off
    btPress(KEY_A_LETTER + 1);                // the letter b
  }
  else if (key == 'C') {                     // go to a slide number
    typingNumber = true;
    slideNumber = "";
  }
  else if (key == 'D') {                     // end the slideshow (Esc)
    btPress(KEY_ESCAPE);
    showRunning = false;
  }
}
 
// ---------- After C: type a slide number, then # ----------
void handleNumberKey(char key) {
  if (key >= '0' && key <= '9' && slideNumber.length() < 3) {
    slideNumber += key;
  }
  else if (key == '*') {                     // delete
    if (slideNumber.length() > 0) slideNumber.remove(slideNumber.length() - 1);
  }
  else if (key == '#') {                     // go!
    if (slideNumber.toInt() > 0) {
      for (int i = 0; i < slideNumber.length(); i++) {
        btDigit(slideNumber[i]);             // PowerPoint: number + Enter
      }
      btPress(KEY_ENTER);                    //   = jump to that slide
      slide = slideNumber.toInt();
    }
    typingNumber = false;
  }
  else if (key == 'D') {                     // cancel
    typingNumber = false;
  }
}
 
// ---------- Screen ----------
void showScreen() {
  lastDraw = millis();
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("SLIDE REMOTE");
  display.setCursor(104, 0);
  display.print(btConnected() ? "BT" : "--");
 
  if (!btConnected()) {
    display.setCursor(0, 16);
    display.println("Not connected.");
    display.println("On the laptop, pair:");
    display.println("");
    display.println(REMOTE_NAME);
  }
  else if (typingNumber) {
    display.setCursor(0, 16);
    display.println("Go to slide:");
    display.setTextSize(2);
    display.print(slideNumber);
    display.println("_");
    display.setTextSize(1);
    display.setCursor(0, 56);
    display.print("#=Go  *=Del  D=Back");
  }
  else {
    display.setCursor(0, 14);
    display.setTextSize(2);
    display.print("Slide ");
    display.println(slide);
 
    if (showRunning) {                       // the presentation timer
      unsigned long seconds = (millis() - showStart) / 1000;
      char timeText[6];
      sprintf(timeText, "%02lu:%02lu", seconds / 60, seconds % 60);
      display.print(timeText);
    } else {
      display.print("--:--");
    }
    display.setTextSize(1);
    display.setCursor(0, 48);
    display.print("#=Next *=Back A=Start");
    display.setCursor(0, 56);
    display.print("B=Black C=GoTo D=End");
  }
  display.display();
}
