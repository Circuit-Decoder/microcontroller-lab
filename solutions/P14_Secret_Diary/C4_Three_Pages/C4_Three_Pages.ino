#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <EEPROM.h>
#include "MultiTap.h"         // the keyboard from Project 13 (second tab)

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

// ---------- NEW: EEPROM SETUP ----------
#define EEPROM_SIZE 300       // CHALLENGE 4: 3 pages x 100 boxes
#define PAGE_SIZE 100
int page = 0;                 // 0, 1 or 2 (keys 1, 2, 3)
#define NOTE_MAX_LENGTH 90    // the longest diary entry we allow

String correctPIN = "1234";
String enteredPIN = "";
String noteText = "";

// mode tracks which screen we are on:
// 0 = enter PIN, 1 = menu, 2 = writing, 3 = reading
int mode = 0;

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }
  EEPROM.begin(EEPROM_SIZE);  // reserve our block of permanent memory
  mtMaxLength = NOTE_MAX_LENGTH;
  display.setTextColor(SSD1306_WHITE);
  showEnterPINScreen();
}

void loop() {
  char key = keypad.getKey();

  if (mode == 2) {            // writing: the keyboard needs EVERY loop
    handleWriting(key);
    return;
  }
  if (!key) return;

  if (mode == 0) {
    handlePINEntry(key);
  }
  else if (mode == 1) {
    handleMenu(key);
  }
  else if (mode == 3) {       // any key: back to the menu
    mode = 1;
    showMenuScreen();
  }
}

void handlePINEntry(char key) {
  if (key == '#') {
    if (enteredPIN == correctPIN) {
      mode = 1;
      showMenuScreen();
    } else {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.println("Wrong PIN");
      display.display();
      delay(1500);
      enteredPIN = "";
      showEnterPINScreen();
    }
  }
  else if (key == '*') {
    enteredPIN = "";
    showEnterPINScreen();
  }
  else {
    enteredPIN += key;
    showEnterPINScreen();
  }
}

void handleMenu(char key) {
  if (key == 'A') {           // write a new entry
    mtClear();
    mode = 2;
    showWritingScreen();
  }
  else if (key == 'B') {      // read the saved entry
    loadNoteFromEEPROM();
    mode = 3;
    showViewScreen();
  }
  else if (key >= '1' && key <= '3') {   // CHALLENGE 4: choose a page
    page = key - '1';
    showMenuScreen();
  }
  else if (key == 'C') {      // add more to the saved entry
    loadNoteFromEEPROM();
    mtClear();
    mtText = noteText;
    mode = 2;
    showWritingScreen();
  }
}

void handleWriting(char key) {
  if (mtUpdate(key)) {        // the keyboard changed something
    showWritingScreen();
  }
  if (key == '#') {           // # = save
    mtLockIn();
    noteText = mtText;
    saveNoteToEEPROM();
    mode = 1;
    showMenuScreen();
  }
  else if (key == 'D') {      // D = cancel without saving
    mode = 1;
    showMenuScreen();
  }
}

// ---------- EEPROM READ / WRITE ----------
void saveNoteToEEPROM() {
  for (int i = 0; i < noteText.length(); i++) {
    EEPROM.write(page * PAGE_SIZE + i, noteText[i]);   // each page has its own area
  }
  EEPROM.write(page * PAGE_SIZE + noteText.length(), '\0');
  EEPROM.commit();                         // really save it now!
}

void loadNoteFromEEPROM() {
  noteText = "";
  for (int i = 0; i < NOTE_MAX_LENGTH; i++) {
    char c = EEPROM.read(page * PAGE_SIZE + i);
    if (c == '\0' || (byte)c == 255) break;   // stop at the end of the text
    noteText += c;
  }
}

// ---------- SCREENS ----------
void showEnterPINScreen() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Enter PIN:");
  display.println(enteredPIN);
  display.setTextSize(1);
  display.println("# = Confirm");
  display.display();
}

void showMenuScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("My Secret Diary  p.");
  display.println(page + 1);
  display.println("1/2/3 = choose page");
  display.println("A = Write new entry");
  display.println("B = Read my diary");
  display.println("C = Add more");
  display.display();
}

void showWritingScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(mtPreview());
  display.println("_");
  display.setCursor(0, 56);
  display.print("#=Save *=Del D=Back");
  display.display();
}

void showViewScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(noteText.length() > 0 ? noteText : "(empty)");
  display.setCursor(0, 56);
  display.print("Any key = Back");
  display.display();
}
