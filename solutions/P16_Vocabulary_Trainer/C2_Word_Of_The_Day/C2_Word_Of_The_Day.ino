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
  String example;         // CHALLENGE 2: a new field - an example sentence
};

// An array of structs: 8 flashcards. Edit them with your own words!
Flashcard cards[8] = {
  {"ABUNDANT", "Very little", "Plenty", "Expensive", "Broken", 'B', "Rice is abundant in Bangladesh."},
  {"DILIGENT", "Hard-working", "Lazy", "Clever", "Angry", 'A', "A diligent student studies daily."},
  {"FRAGILE", "Very heavy", "Very loud", "Easily broken", "Cheap", 'C', "Glass is fragile."},
  {"ANCIENT", "Very modern", "Very fast", "Very small", "Very old", 'D', "Mahasthangarh is ancient."},
  {"GENEROUS", "Selfish", "Giving freely", "Quiet", "Afraid", 'B', "He is generous to the poor."},
  {"RELUCTANT", "Unwilling", "Excited", "Careful", "Famous", 'A', "She was reluctant to go."},
  {"VIVID", "Dull", "Hidden", "Bright, clear", "Wet", 'C', "The sky had vivid colours."},
  {"ACCURATE", "Too late", "Very large", "Noisy", "Exactly right", 'D', "The clock is accurate."}
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
  randomSeed(analogRead(34));
  showWordOfTheDay();                // CHALLENGE 2: first, the word of the day
  while (!keypad.getKey()) { }       // wait for any key
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

// CHALLENGE 2: one random word, its meaning and an example sentence
void showWordOfTheDay() {
  Flashcard card = cards[random(0, TOTAL_CARDS)];
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Word of the Day:");
  display.setCursor(0, 16);   // row 16: below the yellow strip
  display.setTextSize(2);
  display.println(card.word);
  display.setTextSize(1);
  display.println(choiceText(card, card.correctChoice));
  display.println(card.example);
  display.setCursor(0, 56);
  display.print("Any key = start");
  display.display();
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
  display.setCursor(0, 16);   // row 16: below the yellow strip
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
  display.println(" means:");
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
