// Project 05, Bonus 3 - Rock, Paper, Scissors
// # starts a game: press 1, 2 or 3; the ESP32 picks at random.
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
String moves[3] = {"Rock", "Paper", "Scissors"};
bool playing = false;      // waiting for 1, 2 or 3?

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

  if (playing && key >= '1' && key <= '3') {
    playGame(key - '1');     // 0, 1 or 2
    playing = false;
    return;
  }
  playing = false;           // any other key ends the game

  if (key == '#') {
    playing = true;
    showText("Rock Paper Scissors", "1 Rock\n2 Paper\n3 Scissors");
  } else if (key == 'A') {
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
  display.println("#: Rock-paper-sc.");
  display.display();
}

void playGame(int you) {
  int me = random(0, 3);
  String verdict;
  if (you == me) {
    verdict = "Draw!";
  } else if ((you + 1) % 3 == me) {    // the move after yours beats it
    verdict = "I win!";
  } else {
    verdict = "You win!";
  }
  showText("You: " + moves[you],
           "Me: " + moves[me] + "\n\n" + verdict);
}

void showText(String title, String body) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(title);
  display.setCursor(0, 24);
  display.println(body);
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
