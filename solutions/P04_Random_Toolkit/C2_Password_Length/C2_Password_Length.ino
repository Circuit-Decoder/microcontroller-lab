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

int passwordLength = 6;   // CHALLENGE 2: chosen with keys 4-9

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }
  // "Seed" the random generator with a little electrical noise
  // from an unused pin, so the results are different every time.
  randomSeed(analogRead(34));

  display.setTextColor(SSD1306_WHITE);
  showMenu();
}

void loop() {
  char key = keypad.getKey();
  if (!key) return;

  if (key == 'A') {
    rollDice();
  }
  else if (key == 'B') {
    pickLuckyNumber();
  }
  else if (key == 'C') {
    flipCoin();
  }
  else if (key == 'D') {
    generatePassword();
  }
  else if (key == '*') {
    showMenu();
  }
  else if (key >= '4' && key <= '9') {   // choose the length first...
    passwordLength = key - '0';
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("Length: ");
    display.println(passwordLength);
    display.println("Now press D");
    display.display();
  }
}

void showMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Random Toolkit:");
  display.println("A: Roll Dice");
  display.println("B: Lucky Number");
  display.println("C: Coin Flip");
  display.println("D: Password Gen");
  display.println("(4-9 then D: length)");
  display.println("* = Back to Menu");
  display.display();
}

void rollDice() {
  int result = random(1, 7);     // 1 to 6 ("stops before max" rule!)
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Dice Roll:");
  display.setTextSize(3);
  display.println(result);
  display.display();
}

void pickLuckyNumber() {
  int result = random(1, 101);   // 1 to 100
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Lucky Number:");
  display.setTextSize(3);
  display.println(result);
  display.display();
}

void flipCoin() {
  int result = random(0, 2);     // 0 or 1
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  if (result == 0) {
    display.println("Heads!");
  } else {
    display.println("Tails!");
  }
  display.display();
}

void generatePassword() {
  // Letters I and O, and numbers 0 and 1, are left out on purpose:
  // they are easy to confuse when you read them on a small screen.
  const char charset[] = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";
  String password = "";
  int charsetLength = 32;        // number of characters in charset

  for (int i = 0; i < passwordLength; i++) {   // ...then D uses it
    int randomIndex = random(0, charsetLength);
    password += charset[randomIndex];
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Random Password:");
  display.setTextSize(2);
  display.println(password);
  display.display();
}
