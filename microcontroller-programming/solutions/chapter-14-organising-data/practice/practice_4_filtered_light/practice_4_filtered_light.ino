// Chapter 14, Practice 4 - Filtered Light
// E14.5's median filter on the LDR, shown on the OLED.

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int SCREEN_WIDTH  = 128;
const int SCREEN_HEIGHT = 64;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int LDR_PIN = 35;
const int N = 5;
int readings[N];

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
  }
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  for (int i = 0; i < N; i++) {
    readings[i] = analogRead(LDR_PIN);
    delay(2);
  }
  int raw = readings[0];
  sortReadings();
  int filtered = readings[N / 2];

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Light (median of 5)");
  display.print("raw: ");
  display.println(raw);
  display.setTextSize(3);
  display.setCursor(0, 30);
  display.println(filtered);
  display.display();
  delay(100);
}

void sortReadings() {
  for (int pass = 0; pass < N - 1; pass++) {
    for (int i = 0; i < N - 1 - pass; i++) {
      if (readings[i] > readings[i + 1]) {
        int temp = readings[i];
        readings[i] = readings[i + 1];
        readings[i + 1] = temp;
      }
    }
  }
}
