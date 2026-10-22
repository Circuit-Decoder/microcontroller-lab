# Solutions — Project 3: PIN Lock

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Challenge 1: Make the PIN 6 digits long

Full sketch: [`C1_Six_Digit_PIN/C1_Six_Digit_PIN.ino`](C1_Six_Digit_PIN/C1_Six_Digit_PIN.ino)

Change `correctPIN` to 6 digits. As a bonus, `else if (enteredPIN.length() < 6)` stops the user typing more than 6 keys.

## Challenge 2: Show * instead of the real digits

Full sketch: [`C2_Hide_Digits/C2_Hide_Digits.ino`](C2_Hide_Digits/C2_Hide_Digits.ino)

Instead of `display.println(enteredPIN);`, a `for` loop prints one `*` for each character: `enteredPIN.length()` tells how many there are. (You will learn `for` loops properly in Project 4.)

## Challenge 3: Lock for 10 seconds after 3 wrong tries

Full sketch: [`C3_Lockout/C3_Lockout.ino`](C3_Lockout/C3_Lockout.ino)

A counter `wrongTries` goes up by 1 for each wrong PIN and back to 0 for a correct one. When it reaches 3, the screen shows "Locked! Wait 10 s" and `delay(10000);` makes the ESP32 ignore everything for 10 seconds.

## Challenge 4: Touch PIN (extra hard)

Full sketch: [`C4_Touch_PIN/C4_Touch_PIN.ino`](C4_Touch_PIN/C4_Touch_PIN.ino)

Connect a bare wire to GPIO 15. `touchRead(T3)` gives a big number (about 60–80) when nobody touches it, and a small one (about 10–20) when you do. The moment you start touching, the code adds a secret `T` to the PIN, so the correct PIN is 1, 2, 3, 4, touch, then #. If it does not react, open the Serial Monitor, print `touchRead(T3)` and choose a better `TOUCH_LIMIT`.

← [All solutions](../README.md) · [Project code](../../P03_PIN_Lock), [Upgrade](../../P03_PIN_Lock_Upgrade)
