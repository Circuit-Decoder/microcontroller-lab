# Solutions — Project 9: Number System Converter

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Challenge 1: Hex to decimal (and how to type E and F)

Full sketch: [`C1_Hex_To_Decimal/C1_Hex_To_Decimal.ino`](C1_Hex_To_Decimal/C1_Hex_To_Decimal.ino)

Tool D lets 0–9 and A–D go straight into the number. For E and F we borrowed an idea from phones: `*` is a SHIFT key. After `*`, A types E, B types F, C deletes and D goes back to the menu. `hexToDecimal()` works like `binaryToDecimal()`, but multiplies by 16. A letter's value is `h - 'A' + 10`, so 'A' is 10 and 'F' is 15.

## Challenge 2: Binary addition

Full sketch: [`C2_Binary_Addition/C2_Binary_Addition.ino`](C2_Binary_Addition/C2_Binary_Addition.ino)

Tool D asks for two binary numbers. The easiest way to add them is to turn both into normal numbers with `binaryToDecimal()`, add, and turn the sum back into binary with `toBase(sum, 2)`. The screen also shows the decimal check, like 5 + 3 = 8. (To do it bit by bit, like on paper, try it yourself: add from the right and carry the 1!)

## Challenge 3: Morse code for digit keys

Full sketch: [`C3_Morse_Code/C3_Morse_Code.ino`](C3_Morse_Code/C3_Morse_Code.ino)

A `String` array holds the Morse code of 0–9, and `morse[key - '0']` picks the right one — the same `key - '0'` trick again.

## Challenge 4: How many bits a number needs

Full sketch: [`C4_Bits_Needed/C4_Bits_Needed.ino`](C4_Bits_Needed/C4_Bits_Needed.ino)

The number of bits is simply how long its binary text is: `toBase(n, 2).length()`. 25 = 11001 needs 5 bits, 255 = 11111111 needs 8.

← [All solutions](../README.md) · [Project code](../../P09_Number_System_Converter)
