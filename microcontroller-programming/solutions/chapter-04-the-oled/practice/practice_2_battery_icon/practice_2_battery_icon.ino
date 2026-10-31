// Chapter 4, Practice 2 - Battery Icon
// A battery: an outline, a small nub on the end, filled to three-quarters.

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int SCREEN_WIDTH  = 128;
const int SCREEN_HEIGHT = 64;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
  }
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Battery: 75%");
  display.drawRect(20, 24, 80, 30, SSD1306_WHITE);   // body
  display.fillRect(100, 32, 6, 14, SSD1306_WHITE);   // nub on the end
  display.fillRect(23, 27, 56, 24, SSD1306_WHITE);   // 3/4 of 74 inside
  display.display();
}

void loop() {
}
