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

// ---------- QUESTIONS: an array of structs ----------
struct Question {
  int category;            // 1 = General knowledge, 2 = Science, 3 = English
  String text;
  String correct;          // the right answer
  String wrong1, wrong2, wrong3;
};

Question questions[15] = {
  {1, "Bangladesh's capital?", "Dhaka", "Chattogram", "Sylhet", "Khulna"},
  {1, "National flower of\nBangladesh?", "Water lily", "Rose", "Sunflower", "Lotus"},
  {1, "Largest ocean?", "Pacific", "Atlantic", "Indian", "Arctic"},
  {1, "Bangladesh became\nindependent in?", "1971", "1952", "1947", "1975"},
  {1, "How many continents?", "7", "5", "6", "8"},
  {2, "Formula of water?", "H2O", "CO2", "O2", "NaCl"},
  {2, "Planet nearest the\nSun?", "Mercury", "Venus", "Earth", "Mars"},
  {2, "SI unit of force?", "Newton", "Joule", "Watt", "Pascal"},
  {2, "pH of pure water?", "7", "0", "1", "14"},
  {2, "Gas plants take in?", "Carbon dioxide", "Oxygen", "Nitrogen", "Helium"},
  {3, "Past tense of GO?", "Went", "Goed", "Gone", "Going"},
  {3, "Plural of CHILD?", "Children", "Childs", "Childes", "Childrens"},
  {3, "Opposite of ANCIENT?", "Modern", "Old", "Antique", "Early"},
  {3, "Correct spelling?", "Necessary", "Neccessary", "Necesary", "Neccesary"},
  {3, "Same meaning as BIG?", "Large", "Tiny", "Thin", "Short"}
};
const int TOTAL_QUESTIONS = 15;
String categoryNames[4] = {"", "General knowledge", "Science", "English"};

// ---------- LEADERBOARD: top 3, saved in EEPROM ----------
#define NAME_LEN 3
#define ENTRY_SIZE (NAME_LEN + 1)     // 3 letters + 1 score byte
#define NUM_ENTRIES 3
String leaderName[NUM_ENTRIES];
int leaderScore[NUM_ENTRIES];

// ---------- GAME STATE ----------
// mode: 0 = menu, 1 = quiz, 2 = typing a name, 3 = leaderboard
int mode = 0;
int category = 1;
int questionIndex = -1;      // which question is showing now
int asked = 0;               // how many questions asked this round
int score = 0;
bool showingResult = false;
bool wasCorrect = false;
String shuffled[4];          // the 4 answers in random order

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }
  EEPROM.begin(ENTRY_SIZE * NUM_ENTRIES);
  randomSeed(analogRead(34));
  loadLeaderboard();
  mtMaxLength = NAME_LEN;
  display.setTextColor(SSD1306_WHITE);
  showMenu();
}

void loop() {
  char key = keypad.getKey();

  if (mode == 1 && category == 3 && !showingResult) {   // CHALLENGE 3: typed answers
    handleTypedAnswer(key);
    return;
  }
  if (mode == 2) {                      // typing a name: keyboard every loop
    handleNameEntry(key);
    return;
  }
  if (!key) return;

  if (mode == 0) handleMenu(key);
  else if (mode == 1) handleQuiz(key);
  else if (mode == 3) { mode = 0; showMenu(); }
}

void handleMenu(char key) {
  if (key >= '1' && key <= '3') {       // choose a category and start
    category = key - '0';
    asked = 0;
    score = 0;
    questionIndex = -1;
    mode = 1;
    nextQuestion();
  }
  else if (key == '#') {
    mode = 3;
    showLeaderboard();
  }
}

void handleQuiz(char key) {
  if (showingResult) {                  // any key: go on
    showingResult = false;
    if (asked == 5) finishRound();
    else nextQuestion();
    return;
  }
  if (key >= 'A' && key <= 'D') {
    int chosen = key - 'A';             // A -> 0, B -> 1 ...
    wasCorrect = (shuffled[chosen] == questions[questionIndex].correct);
    if (wasCorrect) score++;
    showingResult = true;
    showResultScreen();
  }
}

// Find the next question in the chosen category, then shuffle its answers
void nextQuestion() {
  do {
    questionIndex++;
  } while (questions[questionIndex].category != category);
  asked++;

  Question q = questions[questionIndex];
  String all[4] = {q.correct, q.wrong1, q.wrong2, q.wrong3};
  bool used[4] = {false, false, false, false};
  for (int i = 0; i < 4; i++) {
    int pos;
    do {
      pos = random(0, 4);               // pick a random place...
    } while (used[pos]);                // ...that is still free
    used[pos] = true;
    shuffled[pos] = all[i];
  }
  if (category == 3) {                  // English: the player TYPES the answer
    mtClear();
    mtMaxLength = 12;
    showTypedQuestion();
    return;
  }
  showQuestion();
}

void finishRound() {
  if (score > leaderScore[NUM_ENTRIES - 1]) {   // good enough for the top 3?
    mtClear();
    mtMaxLength = NAME_LEN;             // back to 3 letters for the name
    mode = 2;
    showNameScreen();
  } else {
    mode = 3;
    showLeaderboard();
  }
}

void handleNameEntry(char key) {
  if (mtUpdate(key)) showNameScreen();
  if (key == '#') {
    mtLockIn();
    if (mtText.length() == 0) return;   // type at least one letter
    insertScore(mtText, score);
    mode = 3;
    showLeaderboard();
  }
}

// ---------- LEADERBOARD LOGIC ----------
void insertScore(String name, int newScore) {
  for (int i = 0; i < NUM_ENTRIES; i++) {
    if (newScore > leaderScore[i]) {
      for (int j = NUM_ENTRIES - 1; j > i; j--) {   // shift lower ones down
        leaderName[j] = leaderName[j - 1];
        leaderScore[j] = leaderScore[j - 1];
      }
      leaderName[i] = name;                         // put the new one here
      leaderScore[i] = newScore;
      break;
    }
  }
  saveLeaderboard();
}

void saveLeaderboard() {
  for (int i = 0; i < NUM_ENTRIES; i++) {
    int base = i * ENTRY_SIZE;
    for (int k = 0; k < NAME_LEN; k++) {
      char c = (k < leaderName[i].length()) ? leaderName[i][k] : ' ';
      EEPROM.write(base + k, c);
    }
    EEPROM.write(base + NAME_LEN, leaderScore[i]);
  }
  EEPROM.commit();
}

void loadLeaderboard() {
  for (int i = 0; i < NUM_ENTRIES; i++) {
    int base = i * ENTRY_SIZE;
    leaderName[i] = "";
    byte s = EEPROM.read(base + NAME_LEN);
    // A real score is 0 to 5. Anything else (255 = never written, or old
    // data from another project) means this place is empty.
    bool empty = (s > 5);
    for (int k = 0; k < NAME_LEN; k++) {
      byte c = EEPROM.read(base + k);
      leaderName[i] += empty ? '-' : (char)c;
    }
    leaderScore[i] = empty ? 0 : s;
  }
}

// ---------- SCREENS ----------
void showMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("QUIZ MASTER");
  display.println("");
  display.println("1: General knowledge");
  display.println("2: Science");
  display.println("3: English");
  display.println("");
  display.println("#: Leaderboard");
  display.display();
}

void showQuestion() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Q");
  display.print(asked);
  display.print("/5  Score: ");
  display.println(score);
  display.println(questions[questionIndex].text);
  display.setCursor(0, 32);
  for (int i = 0; i < 4; i++) {
    display.print((char)('A' + i));     // 'A' + 1 = 'B' (ASCII!)
    display.print(":");
    display.println(shuffled[i]);
  }
  display.display();
}

void showResultScreen() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(wasCorrect ? "Correct!" : "Wrong!");
  display.setTextSize(1);
  display.println("");
  display.println("Answer:");
  display.println(questions[questionIndex].correct);
  display.println("");
  display.print("Any key = go on");
  display.display();
}

void showNameScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("TOP 3! Score: ");
  display.println(score);
  display.println("Your name (3 letters)");
  display.setTextSize(2);
  display.print(mtPreview());
  display.println("_");
  display.setTextSize(1);
  display.setCursor(0, 56);
  display.print("# = Save  * = Del");
  display.display();
}

void showLeaderboard() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("LEADERBOARD");
  display.println("");
  for (int i = 0; i < NUM_ENTRIES; i++) {
    display.print(i + 1);
    display.print(". ");
    display.print(leaderName[i]);
    display.print("  ");
    display.print(leaderScore[i]);
    display.println("/5");
  }
  display.setCursor(0, 56);
  display.print("Any key = menu");
  display.display();
}

// ---------- CHALLENGE 3: TYPED ANSWERS FOR ENGLISH ----------
void handleTypedAnswer(char key) {
  if (mtUpdate(key)) showTypedQuestion();
  if (key == '#') {
    mtLockIn();
    if (mtText.length() == 0) return;
    String right = questions[questionIndex].correct;
    right.toUpperCase();                // the keyboard types CAPITALS only
    wasCorrect = (mtText == right);
    if (wasCorrect) score++;
    showingResult = true;
    showResultScreen();
  }
}

void showTypedQuestion() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Q");
  display.print(asked);
  display.print("/5  Score: ");
  display.println(score);
  display.println(questions[questionIndex].text);
  display.println("Type the answer:");
  display.setTextSize(2);
  display.print(mtPreview());
  display.println("_");
  display.setTextSize(1);
  display.setCursor(0, 56);
  display.print("#=Check *=Del");
  display.display();
}
