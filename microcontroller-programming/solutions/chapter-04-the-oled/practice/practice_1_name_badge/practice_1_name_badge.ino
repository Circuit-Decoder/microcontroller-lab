// Chapter 4, Practice 1 - Name Badge
// Your class or school small in the yellow strip, your name big below.

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
  display.println("Class 8, Dhaka");   // yellow strip (rows 0-15)
  display.setTextSize(2);
  display.setCursor(0, 24);            // blue area
  display.println("Rahim");            // your name here
  display.display();
}

void loop() {
}
