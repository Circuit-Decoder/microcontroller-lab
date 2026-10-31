# Microcontroller Programming: Exercise Solutions

Worked solutions for the exercises in the book *Microcontroller Programming*
(Circuit Decoder). The book gives full code only for its experiments and project
references. This folder gives one tested answer for each task that has no
printed code:

- **Practice (Level 1)** tasks at the end of each chapter
- **Bonus challenges (Level 3)** at the end of each project

Try each task yourself first. A solution is *one* way to do it, not the only way.

## Layout

```
solutions/
  chapter-NN-<name>/practice/practice_<n>_<task>/   Level 1 practice
  project-NN-<name>/bonus/bonus_<n>_<task>/         Level 3 bonus
```

Each task folder holds a `README.md` (the task and a short note) and a sketch
with the same name as the folder, so it opens directly in the Arduino IDE.
Some tasks are questions or design jobs rather than code; those folders only
have a README.

## Board and wiring

Every sketch is written for the ESP32 DevKit wiring used in the book:

| Part | Pins |
|---|---|
| Keypad rows / columns | 13, 12, 14, 27 / 26, 25, 33, 32 |
| OLED (I2C, 0x3C) | SDA 21, SCL 22 |
| Green LED / Red LED | 18 / 19 |
| Buzzer | 4 |
| Button A / Button B | 15 / 23 (to GND, `INPUT_PULLUP`) |
| Potentiometer / LDR | 34 / 35 |
| DHT11 | 5 |
| HC-SR04 trigger / echo | 2 / 39 (echo through a divider) |

All sketches compile with the ESP32 Arduino core 3.0.x and the libraries
listed in the book (Keypad, Adafruit SSD1306 + GFX, DHT sensor library,
WebSockets, UniversalTelegramBot, ArduinoJson).

The older lab-book solutions at the top of this repository are untouched.
