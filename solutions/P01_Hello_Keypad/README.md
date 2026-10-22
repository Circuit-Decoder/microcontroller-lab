# Solutions — Project 1: Hello Keypad

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Challenge 1: Show "Hello, Engineer!" for 2 seconds

Full sketch: [`C1_Welcome_Screen/C1_Welcome_Screen.ino`](C1_Welcome_Screen/C1_Welcome_Screen.ino)

In `setup()`, draw the welcome text, call `display.display()`, then `delay(2000);`. After that, draw "Press a key!" as before. `delay()` stops the program for the given number of milliseconds (1000 ms = 1 second).

## Challenge 2: Make the * key clear the screen

Full sketch: [`C2_Star_Clears/C2_Star_Clears.ino`](C2_Star_Clears/C2_Star_Clears.ino)

Check for `*` first with `if (key == '*')`, and draw the start screen again. Use `else if (key)` for all other keys, so `*` is not shown as a pressed key.

## Challenge 3: Show every key in a row, like 12345

Full sketch: [`C3_Show_All_Keys/C3_Show_All_Keys.ino`](C3_Show_All_Keys/C3_Show_All_Keys.ino)

Keep a `String typed = "";` at the top. Each time a key is pressed, add it with `typed += key;` and print the whole `typed`. With text size 2, about 10 characters fit on one line, so the code starts again after 40 characters.

← [All solutions](../README.md) · [Project code](../../P01_Hello_Keypad)
