// Chapter 10, Practice 1 - Volume Meter
// The knob lights 0 to 10 blocks across the screen.

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int SCREEN_WIDTH  = 128;
const int SCREEN_HEIGHT = 64;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int POT_PIN = 34;

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
  }
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  int reading = analogRead(POT_PIN);
  int blocks = map(reading, 0, 4095, 0, 10);   // 0 to 10 blocks

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Volume: ");
  display.println(blocks);
  for (int i = 0; i < 10; i++) {
    int x = i * 12 + 4;                          // 10 px block, 2 px gap
    if (i < blocks) {
      display.fillRect(x, 30, 10, 20, SSD1306_WHITE);   // lit
    } else {
      display.drawRect(x, 30, 10, 20, SSD1306_WHITE);   // empty
    }
  }
  display.display();
  delay(50);
}
