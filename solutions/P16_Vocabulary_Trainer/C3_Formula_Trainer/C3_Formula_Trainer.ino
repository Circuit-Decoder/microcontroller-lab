#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <EEPROM.h>

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

// ---------- NEW: A STRUCT FOR ONE FLASHCARD ----------
struct Flashcard {
  String word;
  String choiceA;
  String choiceB;
  String choiceC;
  String choiceD;
  char correctChoice;     // 'A', 'B', 'C' or 'D'
};

// An array of structs: 8 flashcards. Edit them with your own words!
// CHALLENGE 3: physics formulas instead of words.
// Keep each "word" short (10 letters) and each choice short (19 letters).
Flashcard cards[8] = {
  {"Speed =", "d x t", "d / t", "t / d", "d + t", 'B'},
  {"Force =", "m x a", "m / a", "a / m", "m + a", 'A'},
  {"Density =", "V / m", "m x V", "m / V", "m - V", 'C'},
  {"Pressure", "F x A", "A / F", "F + A", "F / A", 'D'},
  {"Work =", "F / s", "F x s", "s / F", "F - s", 'B'},
  {"Power =", "W / t", "W x t", "t / W", "W + t", 'A'},
  {"Weight =", "m / g", "g / m", "m x g", "m + g", 'C'},
  {"V (Ohm) =", "I / R", "R / I", "I + R", "I x R", 'D'}
};
const int TOTAL_CARDS = 8;

int currentCard = 0;
int score = 0;
int bestScore = 0;            // loaded from EEPROM
bool showingResult = false;
bool wasCorrect = false;

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }
  EEPROM.begin(4);
  bestScore = EEPROM.read(0);
  if (bestScore > TOTAL_CARDS) bestScore = 0;   // empty EEPROM reads 255
  display.setTextColor(SSD1306_WHITE);
  showQuestion();
}

void loop() {
  char key = keypad.getKey();
  if (!key) return;

  if (showingResult) {          // any key: next card
    goToNextCard();
    return;
  }
  if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
    checkAnswer(key);
  }
}

// Returns the text of choice A, B, C or D of a card
String choiceText(Flashcard card, char letter) {
  if (letter == 'A') return card.choiceA;
  if (letter == 'B') return card.choiceB;
  if (letter == 'C') return card.choiceC;
  return card.choiceD;
}

void checkAnswer(char chosen) {
  wasCorrect = (chosen == cards[currentCard].correctChoice);
  if (wasCorrect) {
    score++;
  }
  showingResult = true;
  showResultScreen();
}

void goToNextCard() {
  currentCard++;
  showingResult = false;
  if (currentCard >= TOTAL_CARDS) {   // the quiz is over
    showFinalScore();
    delay(4000);
    currentCard = 0;
    score = 0;
  }
  showQuestion();
}

void showQuestion() {
  Flashcard card = cards[currentCard];   // a copy of the current card
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Card ");
  display.print(currentCard + 1);
  display.print("/");
  display.print(TOTAL_CARDS);
  display.print("   Best: ");
  display.println(bestScore);
  display.setTextSize(2);
  display.println(card.word);
  display.setTextSize(1);
  display.print("A:"); display.println(card.choiceA);
  display.print("B:"); display.println(card.choiceB);
  display.print("C:"); display.println(card.choiceC);
  display.print("D:"); display.println(card.choiceD);
  display.display();
}

void showResultScreen() {
  Flashcard card = cards[currentCard];
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  if (wasCorrect) {
    display.println("Correct!");
  } else {
    display.println("Wrong!");
  }
  display.setTextSize(1);
  display.println("");
  display.print(card.word);
  display.println(" is:");
  display.print(card.correctChoice);
  display.print(": ");
  display.println(choiceText(card, card.correctChoice));
  display.println("");
  display.print("Score: ");
  display.println(score);
  display.print("Any key = next card");
  display.display();
}

void showFinalScore() {
  bool newBest = score > bestScore;
  if (newBest) {                      // save the new best score for ever
    bestScore = score;
    EEPROM.write(0, bestScore);
    EEPROM.commit();
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Quiz complete!");
  display.println("");
  display.setTextSize(2);
  display.print(score);
  display.print(" / ");
  display.println(TOTAL_CARDS);
  display.setTextSize(1);
  display.println("");
  if (newBest) {
    display.println("NEW BEST SCORE!");
  } else {
    display.print("Best score: ");
    display.println(bestScore);
  }
  display.display();
}
