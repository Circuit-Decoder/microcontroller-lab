// Project 06, Bonus 4 - Night Counter
// Counts how many times the lamp has come on since start-up.

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int SCREEN_WIDTH  = 128;
const int SCREEN_HEIGHT = 64;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int LDR_PIN   = 35;
const int POT_PIN   = 34;
const int LAMP      = 18;   // green LED
const int BUTTON_A  = 15;
const int GAP       = 200;  // hysteresis: off level = on level + GAP

bool lampOn = false;
bool autoMode = true;
int brightness = 0;         // what the lamp shows now, 0-255
int lastButton = HIGH;
int nights = 0;             // times the lamp has switched on

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
  }
  display.setTextColor(SSD1306_WHITE);
  pinMode(LAMP, OUTPUT);
  pinMode(BUTTON_A, INPUT_PULLUP);
}

void loop() {
  int light = analogRead(LDR_PIN);
  int onLevel = map(analogRead(POT_PIN), 0, 4095, 200, 3000);

  int button = digitalRead(BUTTON_A);
  if (button == LOW && lastButton == HIGH) {
    autoMode = !autoMode;
  }
  lastButton = button;

  if (!autoMode) {
    lampOn = false;
  } else if (!lampOn && light < onLevel) {
    lampOn = true;
    nights++;                        // only on the change off -> on
  } else if (lampOn && light > onLevel + GAP) {
    lampOn = false;
  }

  int target = 0;
  if (lampOn) {
    target = 255;
  }
  if (brightness < target) {
    brightness = brightness + 5;     // fade in
  } else if (brightness > target) {
    brightness = brightness - 5;     // fade out
  }
  analogWrite(LAMP, brightness);

  showStatus(light, onLevel);
  delay(20);
}

void showStatus(int light, int onLevel) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Night Lamp: ");
  if (autoMode) {
    display.println("AUTO");
  } else {
    display.println("OFF");
  }
  display.print("Lamp ");
  if (lampOn) {
    display.println("ON");
  } else {
    display.println("off");
  }
  display.setCursor(0, 24);
  display.print("Light:  ");
  display.println(light);
  display.print("On below: ");
  display.println(onLevel);
  display.print("Off above: ");
  display.println(onLevel + GAP);
  display.print("Times on: ");
  display.println(nights);
  display.display();
}
