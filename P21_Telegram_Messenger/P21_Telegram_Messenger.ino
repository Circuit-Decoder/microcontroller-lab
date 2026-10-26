#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>      // NEW: an encrypted (https) connection
#include <UniversalTelegramBot.h>  // NEW: library "UniversalTelegramBot" by Brian Lough
#include "MultiTap.h"              // our keyboard from Project 13

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

// ---------- CHANGE THESE ----------
const char* WIFI_NAME     = "YourWiFiName";
const char* WIFI_PASSWORD = "YourPassword";
const char* BOT_TOKEN     = "123456789:ABCdefGhIJKlmNoPQRsTUVwxyZ";  // from BotFather
const String MY_CHAT_ID   = "000000000";   // your own chat ID (the OLED shows it)

WiFiClientSecure client;                   // the secure connection
UniversalTelegramBot bot(BOT_TOKEN, client);

const unsigned long CHECK_EVERY = 3000;    // ask Telegram for news every 3 s
unsigned long lastCheck = 0;

String lastMessage = "";
String lastSender = "";
int messageCount = 0;
bool typingReply = false;
bool showingBlocked = false;           // a blocked chat ID is on the screen

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }
  display.setTextColor(SSD1306_WHITE);
  mtMaxLength = 80;

  connectToWiFi();
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);   // trust only the real Telegram
  bot.sendMessage(MY_CHAT_ID, "Hello! Your ESP32 is online.");
  showScreen();
}

void loop() {
  char key = keypad.getKey();

  if (showingBlocked && key) {           // any key: hide the blocked chat ID
    showingBlocked = false;
    showScreen();
    return;
  }

  if (typingReply) {
    if (mtUpdate(key)) showScreen();
    if (key == '#') {                        // # = send
      mtLockIn();
      if (mtText.length() > 0) {
        showStatus("Sending...");
        bool ok = bot.sendMessage(MY_CHAT_ID, mtText);
        showStatus(ok ? "Sent!" : "Send failed");
        delay(800);
      }
      typingReply = false;
      showScreen();
    }
    else if (key == 'D') {                   // D = cancel
      typingReply = false;
      showScreen();
    }
    return;                                  // don't check messages while typing
  }

  if (key == 'A') {                          // A = write a message
    mtClear();
    typingReply = true;
    showScreen();
  }
  else if (key == 'B') {                     // B = quick reply
    bot.sendMessage(MY_CHAT_ID, "OK, got it!");
    showStatus("Sent: OK, got it!");
    delay(800);
    showScreen();
  }

  // Every 3 seconds: are there new messages?
  if (millis() - lastCheck > CHECK_EVERY) {
    int count = bot.getUpdates(bot.last_message_received + 1);
    while (count > 0) {
      for (int i = 0; i < count; i++) {
        handleMessage(i);
      }
      count = bot.getUpdates(bot.last_message_received + 1);
    }
    lastCheck = millis();
  }
}

// ---------- One new message from Telegram ----------
void handleMessage(int i) {
  String chatId = bot.messages[i].chat_id;
  String text   = bot.messages[i].text;
  String name   = bot.messages[i].from_name;

  // Safety first: only listen to YOUR chat
  if (chatId != MY_CHAT_ID) {
    bot.sendMessage(chatId, "Sorry, this is a private device.");
    showStatus("Blocked chat ID:\n" + chatId + "\n\nAny key = back");
    showingBlocked = true;               // stays until a key is pressed
    return;
  }

  if (text == "/start") {
    bot.sendMessage(chatId, "Hi " + name + "! Send any text and I will show it.\n/status - how am I?");
  }
  else if (text == "/status") {
    String s = "Online for " + String(millis() / 60000) + " min\n";
    s += "Messages received: " + String(messageCount) + "\n";
    s += "Wi-Fi signal: " + String(WiFi.RSSI()) + " dBm";
    bot.sendMessage(chatId, s);
  }
  else {
    lastMessage = text;
    lastSender = name;
    messageCount++;
    bot.sendMessage(chatId, "Shown on the OLED.");
    showScreen();
  }
}

// ---------- Screens ----------
void showScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);

  if (typingReply) {
    display.println("Message to phone:");
    display.print(mtPreview());
    display.println("_");
    display.setCursor(0, 56);
    display.print("#=Send *=Del D=Back");
  }
  else if (messageCount == 0) {
    display.println("TELEGRAM MESSENGER");
    display.println("");
    display.println("Waiting for a");
    display.println("message...");
    display.setCursor(0, 56);
    display.print("A=Write  B=Quick OK");
  }
  else {
    display.print(lastSender);
    display.print(" (");
    display.print(messageCount);
    display.println("):");
    display.drawLine(0, 9, 127, 9, SSD1306_WHITE);
    display.setCursor(0, 16);                 // row 16: below the yellow strip
    display.println(lastMessage);
    display.setCursor(0, 56);
    display.print("A=Reply  B=Quick OK");
  }
  display.display();
}

void showStatus(String text) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.println(text);
  display.display();
}

void connectToWiFi() {
  showStatus("Connecting to Wi-Fi\n" + String(WIFI_NAME));
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - start > 20000) {          // give up after 20 seconds
      showStatus("Wi-Fi failed!\nCheck name/password");
      while (true);
    }
    delay(250);
  }
  showStatus("Wi-Fi connected!");
  delay(1000);
}
