# Solutions — Project 12: Typing Speed Trainer

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Challenge 1: Show the time in seconds

Full sketch: [`C1_Seconds/C1_Seconds.ino`](C1_Seconds/C1_Seconds.ino)

`elapsedMs / 1000.0` turns milliseconds into seconds. The `.0` matters: `3500 / 1000` would give 3, but `3500 / 1000.0` gives 3.5. `display.print(…, 2)` shows 2 decimals.

## Challenge 2: Speed in digits per second

Full sketch: [`C2_Speed/C2_Speed.ino`](C2_Speed/C2_Speed.ino)

Speed = how many digits ÷ how many seconds. 6 digits in 3.5 seconds = 1.71 digits per second.

## Challenge 3: A live clock while you type

Full sketch: [`C3_Live_Clock/C3_Live_Clock.ino`](C3_Live_Clock/C3_Live_Clock.ino)

The update must happen even when no key is pressed, so it goes ABOVE `if (!key) return;`. It runs only every 100 ms (`millis() - lastDraw >= 100`): redrawing on every loop would make the screen flicker.

## Challenge 4: Save the best time (after Project 14)

Full sketch: [`C4_Best_Time_Saved/C4_Best_Time_Saved.ino`](C4_Best_Time_Saved/C4_Best_Time_Saved.ino)

A time can be bigger than 255, so it needs 4 EEPROM boxes. `EEPROM.put(0, bestMs)` writes all 4 at once and `EEPROM.get(0, bestMs)` reads them back. A new best only counts when every digit is correct. Because other projects may have left data in the EEPROM, the code only trusts values between 0.1 and 600 seconds.

← [All solutions](../README.md) · [Project code](../../P12_Typing_Speed_Trainer)
