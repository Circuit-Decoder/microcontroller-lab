# Solutions — Project 13: Old-Phone Keyboard

Each solution folder has its own copy of MultiTap.h, because some challenges change it.

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Challenge 1: Lowercase with the A key

Full sketch: [`C1_Lowercase/C1_Lowercase.ino`](C1_Lowercase/C1_Lowercase.ino)

This solution changes its own copy of `MultiTap.h`: a new `mtLowercase` switch, and `mtCurrentLetter()` turns 'A'–'Z' into small letters by adding 32 ('A' is 65 and 'a' is 97 — Project 9!). In the sketch, A flips the switch.

## Challenge 2: Word counter

Full sketch: [`C2_Word_Counter/C2_Word_Counter.ino`](C2_Word_Counter/C2_Word_Counter.ino)

A new word starts wherever a letter comes right after a space, or at the very start. `countWords()` counts those places, so even two spaces in a row are counted correctly.

## Challenge 3: Speed test: how fast can you type HELLO?

Full sketch: [`C3_Speed_Test/C3_Speed_Test.ino`](C3_Speed_Test/C3_Speed_Test.ino)

The clock starts with `millis()` at the first digit key, and stops when you press #. If `mtText == "HELLO"`, the time is shown in seconds. D clears the text and resets the clock.

## Challenge 4: Your own key map for maths

Full sketch: [`C4_Maths_Key_Map/C4_Maths_Key_Map.ino`](C4_Maths_Key_Map/C4_Maths_Key_Map.ino)

Only one line in `MultiTap.h` changes: key 1 becomes `"+-*/=1"`. The small OLED font has no × or ÷ signs, so we use * and /, just like in code.

← [All solutions](../README.md) · [Project code](../../P13_Old_Phone_Keyboard)
