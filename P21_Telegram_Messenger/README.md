# Project 21: Telegram Messenger

**Send and receive messages from anywhere.**  
Book: *Microcontroller Lab* by Circuit Decoder — Tier 4 — Wireless & connected

## Files

- [`MultiTap.h`](MultiTap.h)
- [`P21_Telegram_Messenger.ino`](P21_Telegram_Messenger.ino)

## How to open it

This project has 2 files. Keep them together in this folder: the Arduino IDE opens them as tabs automatically.

1. Easiest: download the whole repository (green **Code** button → **Download ZIP**) and unzip it.
2. Double-click `P21_Telegram_Messenger.ino`. It opens in the Arduino IDE, with `MultiTap.h` as a second tab.
3. Or copy the code: open the `.ino` file above and click the **Copy raw file** button (top right of the code).

## Libraries needed

- Adafruit GFX Library
- Adafruit SSD1306
- Keypad (by Mark Stanley and Alexander Brevig)
- UniversalTelegramBot by Brian Lough
- ArduinoJson by Benoit Blanchon

Board: **ESP32 Dev Module**, with *esp32 by Espressif Systems* version **3.x**.

## Before you upload

Before uploading, fill in `WIFI_NAME`, `WIFI_PASSWORD` and `BOT_TOKEN` (from @BotFather). Leave `MY_CHAT_ID` as `"000000000"` the first time — the OLED will show your real chat ID. **Never share your token or Wi-Fi password.**

← [All projects](https://github.com/Circuit-Decoder/microcontroller-lab)
