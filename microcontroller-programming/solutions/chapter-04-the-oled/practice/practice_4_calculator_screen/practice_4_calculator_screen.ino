// Chapter 4, Practice 4 - Calculator Screen
// E3.2's five sums on the OLED instead of the Serial Monitor.

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int SCREEN_WIDTH  = 128;
const int SCREEN_HEIGHT = 64;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int apples = 7;
int friends = 2;

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
  }
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Sums with 7 and 2");
  display.setCursor(0, 16);              // blue area
  display.print("7 + 2 = ");
  display.println(apples + friends);
  display.print("7 - 2 = ");
  display.println(apples - friends);
  display.print("7 * 2 = ");
  display.println(apples * friends);
  display.print("7 / 2 = ");
  display.println(apples / friends);
  display.print("7 % 2 = ");
  display.println(apples % friends);
  display.display();
}

void loop() {
}
