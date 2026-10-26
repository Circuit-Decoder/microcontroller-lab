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

// ---------- NEW: CONTACT STORAGE LAYOUT ----------
#define NAME_SLOT_SIZE 10
#define NUMBER_SLOT_SIZE 15
#define CONTACT_SLOT_SIZE (NAME_SLOT_SIZE + NUMBER_SLOT_SIZE)
#define MAX_CONTACTS 5
#define EEPROM_SIZE (CONTACT_SLOT_SIZE * MAX_CONTACTS)

// mode: 0 = menu, 1 = typing a new name, 2 = typing the number,
//       3 = typing a name to search, 4 = showing the result
int mode = 0;
String currentInput = "";
String tempName = "";
int nextEmptySlot = 0;
int listSlot = -1;        // CHALLENGE 1: the slot shown in the list (mode 5)

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }
  EEPROM.begin(EEPROM_SIZE);
  nextEmptySlot = findEmptySlot();   // continue after the saved contacts
  mtMaxLength = NAME_SLOT_SIZE;      // names can be up to 10 letters
  display.setTextColor(SSD1306_WHITE);
  showMenu();
}

void loop() {
  char key = keypad.getKey();

  if (mode == 1 || mode == 3) {      // typing a name: keyboard needs EVERY loop
    handleNameTyping(key);
    return;
  }
  if (!key) return;

  if (key == 'D' && mode != 0) {     // D = cancel, back to the menu
    mode = 0;
    showMenu();
    return;
  }

  if (mode == 0) handleMenu(key);
  else if (mode == 2) handleAddNumber(key);
  else if (mode == 4) { mode = 0; showMenu(); }
  else if (mode == 5 && key == 'C') showNextContact();   // C = next contact
}

void handleMenu(char key) {
  if (key == 'A') {                  // add a contact
    mtClear();
    mode = 1;
    showNameScreen();
  }
  else if (key == 'C') {             // CHALLENGE 1: list all contacts
    listSlot = -1;
    mode = 5;
    showNextContact();
  }
  else if (key == 'B') {             // search for a contact
    mtClear();
    mode = 3;
    showNameScreen();
  }
}

// Used for both adding (mode 1) and searching (mode 3)
void handleNameTyping(char key) {
  if (mtUpdate(key)) {
    showNameScreen();
  }
  if (key == '#') {
    mtLockIn();
    if (mtText.length() == 0) return;          // type a name first
    if (mode == 1) {                           // adding: now the number
      tempName = mtText;
      currentInput = "";
      mode = 2;
      showAddNumberScreen();
    } else {                                   // searching: show the result
      showSearchResult(mtText);
      mode = 4;
    }
  }
  else if (key == 'D') {                       // cancel
    mode = 0;
    showMenu();
  }
}

void handleAddNumber(char key) {
  if (key == '#') {
    saveContact(tempName, currentInput);
    mode = 0;
    showMenu();
  }
  else if (key == '*') {                       // delete the last digit
    if (currentInput.length() > 0) {
      currentInput.remove(currentInput.length() - 1);
    }
    showAddNumberScreen();
  }
  else if (key >= '0' && key <= '9' && currentInput.length() < NUMBER_SLOT_SIZE) {
    currentInput += key;
    showAddNumberScreen();
  }
}

void showSearchResult(String name) {
  String foundNumber = findContact(name);
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  if (foundNumber == "") {
    display.print(name);
    display.println(": not found.");
  } else {
    display.setTextSize(2);
    display.println(name);
    display.setTextSize(1);
    display.println("");
    display.print("Tel: ");
    display.println(foundNumber);
  }
  display.setCursor(0, 56);
  display.print("Any key = Menu");
  display.display();
}

// ---------- EEPROM READ / WRITE HELPERS ----------
void saveContact(String name, String number) {
  int slotStart = nextEmptySlot * CONTACT_SLOT_SIZE;

  // Write the name, padded with '\0' up to NAME_SLOT_SIZE
  for (int i = 0; i < NAME_SLOT_SIZE; i++) {
    char c = (i < name.length()) ? name[i] : '\0';
    EEPROM.write(slotStart + i, c);
  }
  // Write the number right after the name
  for (int i = 0; i < NUMBER_SLOT_SIZE; i++) {
    char c = (i < number.length()) ? number[i] : '\0';
    EEPROM.write(slotStart + NAME_SLOT_SIZE + i, c);
  }
  EEPROM.commit();
  // move on; after the last slot, wrap around to slot 0
  nextEmptySlot = (nextEmptySlot + 1) % MAX_CONTACTS;
}

String readNameFromSlot(int slot) {
  String name = "";
  int slotStart = slot * CONTACT_SLOT_SIZE;
  for (int i = 0; i < NAME_SLOT_SIZE; i++) {
    char c = EEPROM.read(slotStart + i);
    if (c == '\0' || (byte)c == 255) break;
    name += c;
  }
  return name;
}

String readNumberFromSlot(int slot) {
  String number = "";
  int slotStart = slot * CONTACT_SLOT_SIZE;
  for (int i = 0; i < NUMBER_SLOT_SIZE; i++) {
    char c = EEPROM.read(slotStart + NAME_SLOT_SIZE + i);
    if (c == '\0' || (byte)c == 255) break;
    number += c;
  }
  return number;
}

// Linear search: check every slot, one by one
String findContact(String searchName) {
  for (int slot = 0; slot < MAX_CONTACTS; slot++) {
    String storedName = readNameFromSlot(slot);
    if (slotIsUsed(slot) && storedName == searchName) {
      return readNumberFromSlot(slot);
    }
  }
  return "";   // not found
}

// A slot is "used" only if it holds a real phone number (digits only).
// Old data left by other projects (like the diary) counts as empty.
bool slotIsUsed(int slot) {
  String number = readNumberFromSlot(slot);
  if (number.length() == 0) return false;
  for (int i = 0; i < number.length(); i++) {
    if (number[i] < '0' || number[i] > '9') return false;
  }
  return true;
}

// After a restart: find the first slot that is not used yet
int findEmptySlot() {
  for (int slot = 0; slot < MAX_CONTACTS; slot++) {
    if (!slotIsUsed(slot)) return slot;
  }
  return 0;    // all full: start again at slot 0
}

// ---------- SCREENS ----------
void showMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Contact Book");
  display.println("");
  display.println("A: Add contact");
  display.println("B: Search by name");
  display.println("C: List all");
  display.display();
}

void showNameScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  if (mode == 1) display.println("New contact - name:");
  else display.println("Search - name:");
  display.setCursor(0, 16);   // row 16: below the yellow strip
  display.setTextSize(2);
  display.print(mtPreview());
  display.println("_");
  display.setTextSize(1);
  display.setCursor(0, 56);
  display.print("#=OK *=Del D=Back");
  display.display();
}

void showAddNumberScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Number for ");
  display.println(tempName);
  display.setCursor(0, 16);   // row 16: below the yellow strip
  display.setTextSize(2);
  display.println(currentInput);
  display.setTextSize(1);
  display.setCursor(0, 56);
  display.print("#=Save *=Del D=Back");
  display.display();
}

// ---------- CHALLENGE 1: PAGE THROUGH ALL CONTACTS ----------
// Look for the next USED slot after listSlot. Wrap around at the end.
void showNextContact() {
  int found = -1;
  for (int step = 1; step <= MAX_CONTACTS; step++) {
    int slot = (listSlot + step) % MAX_CONTACTS;
    if (slotIsUsed(slot)) { found = slot; break; }
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  if (found == -1) {
    display.println("No contacts yet.");
  } else {
    listSlot = found;
    display.print("Slot ");
    display.println(found);
    display.setCursor(0, 16);   // row 16: below the yellow strip
    display.setTextSize(2);
    display.println(readNameFromSlot(found));
    display.setTextSize(1);
    display.print("Tel: ");
    display.println(readNumberFromSlot(found));
  }
  display.setCursor(0, 56);
  display.print("C=Next  D=Menu");
  display.display();
}
