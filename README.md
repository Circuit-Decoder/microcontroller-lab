# Microcontroller Lab

**23 real projects with an ESP32, a keypad and an OLED screen.**  
The code for the book *Microcontroller Lab* by **Circuit Decoder**.

Every program here is exactly the code printed in the book. Each one was compiled for the ESP32 before publishing.

## Get the code

**Easiest (on a computer):** click the green **Code** button above → **Download ZIP**, then unzip it. Every project is its own folder — double-click the `.ino` file inside to open it in the Arduino IDE. Extra tabs like `MultiTap.h` are already in the right folders.

**One project only:** open its folder below, open the `.ino` file, and click **Copy raw file**. Paste it into a new sketch (File → New Sketch).

## Set up once

1. Install the free [Arduino IDE](https://www.arduino.cc/en/software).
2. In **File → Preferences → Additional boards manager URLs**, add:  
   `https://espressif.github.io/arduino-esp32/package_esp32_index.json`
3. In **Tools → Board → Boards Manager**, install **esp32 by Espressif Systems**, version **3.x** (checked with 3.0.7).
4. Choose **Tools → Board → esp32 → ESP32 Dev Module**.
5. In **Sketch → Include Library → Manage Libraries**, install **Adafruit GFX Library**, **Adafruit SSD1306** and **Keypad** (by Mark Stanley and Alexander Brevig). Projects 20 and 21 need one or two more — see their folders.

## Wiring (the same for every project)

| OLED pin | ESP32 pin |
|---|---|
| GND | GND |
| VCC | 3V3 |
| SCL | GPIO 22 |
| SDA | GPIO 21 |

| Keypad pin (face up, pins at the bottom, from the left) | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
|---|---|---|---|---|---|---|---|---|
| ESP32 pin | 13 | 12 | 14 | 27 | 26 | 25 | 33 | 32 |

## The projects

### Tier 1 — First steps

| # | Project | What you build |
|---|---|---|
| 1 | [Hello Keypad](P01_Hello_Keypad) | Press a key, see it on your screen |
| 2 | [Science Reference — Part A](P02_Science_Reference_PartA) | Physics constants on number keys |
| 2 | [Science Reference — Part B](P02_Science_Reference_PartB) | Type an atomic number, see the element |
| 3 | [PIN Lock](P03_PIN_Lock) | A digital lock that opens only with the right code |
| 3 | [PIN Lock — LED and buzzer upgrade](P03_PIN_Lock_Upgrade) | The optional upgrade, complete |
| 4 | [Random Toolkit](P04_Random_Toolkit) | Dice, coin toss and strong password generator |

### Tier 2 — Maths, science & money

| # | Project | What you build |
|---|---|---|
| 5 | [Smart Calculator](P05_Smart_Calculator) | A real 4-function calculator |
| 5 | [Bill Splitter](P05_Bill_Splitter) | Split a restaurant bill with a tip |
| 6 | [Number Explorer](P06_Number_Explorer) | Prime check, factors, GCD/LCM, even/odd |
| 7 | [Quadratic Equation Solver](P07_Quadratic_Solver) | Roots of ax² + bx + c = 0, step by step |
| 8 | [Statistics Tool](P08_Statistics_Tool) | Mean, median and range of your marks — with sorting |
| 9 | [Number System Converter](P09_Number_System_Converter) | Decimal to binary, octal and hex |
| 10 | [Chemistry Helper](P10_Chemistry_Helper) | Molar mass of compounds |
| 11 | [Graph Plotter](P11_Graph_Plotter) | Type an equation and see its graph |

### Tier 3 — Memory & learning

| # | Project | What you build |
|---|---|---|
| 12 | [Typing Speed Trainer](P12_Typing_Speed_Trainer) | How fast and accurate can you type? |
| 13 | [Old-Phone Keyboard](P13_Old_Phone_Keyboard) | Type real words, Nokia-style |
| 14 | [Secret Diary](P14_Secret_Diary) | PIN-protected notes that survive power off |
| 15 | [Contact Book](P15_Contact_Book) | Save real names and numbers, then search them |
| 16 | [English Vocabulary Trainer](P16_Vocabulary_Trainer) | Flashcards with a saved high score |
| 17 | [Quiz Master](P17_Quiz_Master) | GK, grammar and science quiz with a leaderboard |

### Tier 4 — Wireless & connected

| # | Project | What you build |
|---|---|---|
| 18 | [Internet Clock](P18_Internet_Clock) | Exact Bangladesh time from the internet |
| 19 | [Phone Message Board](P19_Phone_Message_Board) | Type on your phone, see it on your device |
| 20 | [Phone Drawing Pad](P20_Phone_Drawing_Pad) | Draw with your finger, see it on the OLED |
| 21 | [Telegram Messenger](P21_Telegram_Messenger) | Send and receive messages from anywhere |
| 22 | [QR Code Generator](P22_QR_Code_Generator) | Type text, get a QR code any phone can scan |
| 23 | [Bluetooth Presentation Remote](P23_Presentation_Remote) | Change PowerPoint slides from your device |

## Challenge solutions

Every project ends with **Challenge Yourself** tasks. Worked answers for all of them — 93 sketches, each one checked to compile for the ESP32 — are in the [**solutions**](solutions) folder. Try each challenge first, then compare.

## Safety note

Projects 18, 21 and 22 need a Wi-Fi password or a Telegram bot token. Type yours only on your own computer. **Never share code that contains your real password or token.**

---

© 2026 Circuit Decoder. You may use and change this code for your own learning. Please do not republish or sell it.
