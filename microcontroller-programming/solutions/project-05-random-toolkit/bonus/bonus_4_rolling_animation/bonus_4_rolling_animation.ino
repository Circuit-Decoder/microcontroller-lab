// Project 05, Bonus 4 - Rolling Animation
// A flashes ten quick random numbers before the final roll.
// A = dice, B = lucky number 1-100, C = coin, D = 6-character password,
// * = menu. Each press of a tool key gives a new result.

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

// No easily confused characters: no O/0, no I/1
const String CHARSET = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
  }
  display.setTextColor(SSD1306_WHITE);
  showMenu();
}

void loop() {
  char key = keypad.getKey();
  if (!key) return;

  if (key == 'A') {
    for (int i = 0; i < 10; i++) {
      showResult("Rolling...", String(random(1, 7)));
      delay(60 + i * 15);              // slowing down, like a real die
    }
    showResult("Dice Roller", String(random(1, 7)));
  } else if (key == 'B') {
    showResult("Lucky Number", String(random(1, 101)));
  } else if (key == 'C') {
    if (random(0, 2) == 0) {
      showResult("Coin Flip", "Heads");
    } else {
      showResult("Coin Flip", "Tails");
    }
  } else if (key == 'D') {
    showResult("Password", makePassword(6));
  } else if (key == '*') {
    showMenu();
  }
}

String makePassword(int length) {
  String password = "";
  for (int i = 0; i < length; i++) {
    password += CHARSET[random(0, CHARSET.length())];
  }
  return password;
}

void showMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Random Toolkit");
  display.setCursor(0, 16);
  display.println("A: Dice roller");
  display.println("B: Lucky number");
  display.println("C: Coin flip");
  display.println("D: Password");
  display.println("*: Menu");
  display.display();
}

void showResult(String title, String result) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(title);
  display.println("Same key = again");
  display.setTextSize(3);
  display.setCursor(0, 24);
  display.println(result);
  display.display();
  Serial.print(title);
  Serial.print(": ");
  Serial.println(result);
}
