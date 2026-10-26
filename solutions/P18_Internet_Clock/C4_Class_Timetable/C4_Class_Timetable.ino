#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <WiFi.h>              // NEW: the ESP32's built-in Wi-Fi
#include <time.h>              // NEW: date and time functions

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

// ---------- CHANGE THESE: your Wi-Fi name and password ----------
const char* WIFI_NAME     = "YourWiFiName";
const char* WIFI_PASSWORD = "YourPassword";

// ---------- Bangladesh time: UTC + 6 hours ----------
const char* TIME_ZONE = "<+06>-6";
const char* TIME_SERVER = "pool.ntp.org";

// ---------- CHANGE THIS: the day you are counting down to ----------
const char* EVENT_NAME = "SSC exam";
const int EVENT_YEAR = 2027, EVENT_MONTH = 2, EVENT_DAY = 15;

bool use24Hour = true;          // A switches between 24-hour and 12-hour
int screen = 0;                 // 0 = clock, 1 = countdown
unsigned long lastUpdate = 0;

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }
  display.setTextColor(SSD1306_WHITE);
  connectToWiFi();
  configTzTime(TIME_ZONE, TIME_SERVER);     // ask the internet for the time
}

void loop() {
  char key = keypad.getKey();
  if (key == 'A') use24Hour = !use24Hour;   // ! flips true <-> false
  if (key == 'B') screen = 1 - screen;      // 0 -> 1, 1 -> 0
  if (key == 'C') screen = 2;               // CHALLENGE 4: class timetable
  if (key == 'D') screen = 0;

  if (key || millis() - lastUpdate >= 1000) {   // redraw once a second
    lastUpdate = millis();
    struct tm now;
    if (getLocalTime(&now)) {
      if (screen == 0) showClock(now);
      else if (screen == 1) showCountdown(now);
      else showPeriod(now);
    } else {
      showMessage("Getting time...", "from the internet");
    }
  }
}

void connectToWiFi() {
  showMessage("Connecting to Wi-Fi", WIFI_NAME);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - start > 20000) {          // give up after 20 seconds
      showMessage("Wi-Fi failed!", "Check name/password");
      while (true);                          // stop here
    }
    delay(250);
  }
  showMessage("Connected! My IP:", WiFi.localIP().toString().c_str());
  delay(1500);
}

void showClock(struct tm &now) {
  char timeText[12];
  char dateText[20];
  if (use24Hour) strftime(timeText, sizeof(timeText), "%H:%M:%S", &now);
  else           strftime(timeText, sizeof(timeText), "%I:%M %p", &now);
  strftime(dateText, sizeof(dateText), "%a %d %b %Y", &now);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Bangladesh time");
  display.setTextSize(2);
  display.setCursor(0, 18);
  display.println(timeText);
  display.setTextSize(1);
  display.setCursor(0, 42);
  display.println(dateText);
  display.setCursor(0, 56);
  display.print("A=12/24 B=Days C=Class");
  display.display();
}

void showCountdown(struct tm &now) {
  struct tm event = {};                  // all fields start at 0
  event.tm_year = EVENT_YEAR - 1900;     // years are counted from 1900
  event.tm_mon  = EVENT_MONTH - 1;       // months are counted from 0
  event.tm_mday = EVENT_DAY;

  struct tm today = now;                 // today at midnight
  today.tm_hour = 0;
  today.tm_min = 0;
  today.tm_sec = 0;

  long secondsLeft = (long)(mktime(&event) - mktime(&today));
  long daysLeft = secondsLeft / 86400;   // 86400 seconds in a day

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Days until ");
  display.println(EVENT_NAME);
  display.setTextSize(3);
  display.setCursor(0, 20);
  if (daysLeft >= 0) display.println(daysLeft);
  else display.println("Done!");
  display.setTextSize(1);
  display.setCursor(0, 56);
  display.print("B = Clock");
  display.display();
}

void showMessage(const char* line1, const char* line2) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(line1);
  display.println("");
  display.println(line2);
  display.display();
}

// ---------- CHALLENGE 4: CLASS TIMETABLE ----------
// Change these to your own school's timetable.
// Each period starts at startHour:startMinute. The last entry is the end of school.
const int NUM_PERIODS = 7;
String periodNames[NUM_PERIODS] = {"1: Bangla", "2: English", "3: Maths", "Tiffin break",
                                   "4: Science", "5: ICT", "School is over"};
int startHour[NUM_PERIODS]   = {9,  9, 10, 11, 11, 12, 13};
int startMinute[NUM_PERIODS] = {0, 45, 30, 15, 45, 30, 15};

void showPeriod(struct tm &now) {
  int nowMinutes = now.tm_hour * 60 + now.tm_min;
  int current = -1;
  // The current period is the LAST one that has already started
  for (int i = 0; i < NUM_PERIODS; i++) {
    if (startHour[i] * 60 + startMinute[i] <= nowMinutes) current = i;
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Now:");
  display.setCursor(0, 16);   // row 16: below the yellow strip
  display.setTextSize(2);
  if (current == -1) {
    display.println("Before");
    display.println("school");
  } else {
    display.println(periodNames[current]);
  }
  display.setTextSize(1);
  if (current >= 0 && current < NUM_PERIODS - 1) {
    int next = current + 1;
    int left = startHour[next] * 60 + startMinute[next] - nowMinutes;
    display.setCursor(0, 44);
    display.print("Next in ");
    display.print(left);
    display.println(" min");
  }
  display.setCursor(0, 56);
  display.print("D = Clock");
  display.display();
}
