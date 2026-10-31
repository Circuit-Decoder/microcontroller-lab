// Chapter 9, Practice 2 - Bar on Screen
// Ten vertical lines, 10 pixels apart.

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
  for (int i = 0; i < 10; i++) {
    int x = 10 + i * 10;                 // 10, 20, ... 100
    display.drawLine(x, 16, x, 63, SSD1306_WHITE);
  }
  display.display();
}

void loop() {
}
