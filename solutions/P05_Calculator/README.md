# Solutions — Project 5: Calculator

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Exercise: Bill Splitter

Full sketch: [`Bill_Splitter/Bill_Splitter.ino`](Bill_Splitter/Bill_Splitter.ino)

The calculator's stage idea, with four stages instead of three: bill total → number of people → tip percent → result. Each `#` turns `currentInput` into a number with `toInt()`, saves it, and moves to the next stage. The tip uses `tipPercent / 100.0` (not `100`), so the decimals are kept. If the number of people is 0, the sketch shows "Need at least 1 person!" instead of dividing by zero.

Test it with: 1200, 4, 10 → Each: 330.00 · 1000, 3, 0 → Each: 333.33 · 500, 0, 0 → "Need at least 1 person!"

## Challenge 1: Show "Cannot divide by 0"

Full sketch: [`C1_Divide_By_Zero_Message/C1_Divide_By_Zero_Message.ino`](C1_Divide_By_Zero_Message/C1_Divide_By_Zero_Message.ino)

On the result screen, check `operation == '/' && num2 == 0` first. If true, show the message instead of calling `calculateResult()`.

## Challenge 2: Make * work like a backspace

Full sketch: [`C2_Backspace/C2_Backspace.ino`](C2_Backspace/C2_Backspace.ino)

`currentInput.remove(currentInput.length() - 1);` removes the last character. If there is nothing left to delete (or the result is showing), * resets the calculator as before.

## Challenge 3: Show the whole sum, like "12 + 8 = 20"

Full sketch: [`C3_Show_Whole_Sum/C3_Show_Whole_Sum.ino`](C3_Show_Whole_Sum/C3_Show_Whole_Sum.ino)

On the result screen, print `num1`, the `operation` sign and `num2` on one small line, then the answer in big letters. `display.print(num1, 0)` prints the number with 0 decimal places.

## Challenge 4: Bill Splitter (after the exercise): A = 5%, B = 10%, C = 15%

Full sketch: [`C4_Bill_Splitter_Letter_Keys/C4_Bill_Splitter_Letter_Keys.ino`](C4_Bill_Splitter_Letter_Keys/C4_Bill_Splitter_Letter_Keys.ino)

This one changes the Bill Splitter. In stage 3 (the tip), the letter keys put "5", "10" or "15" into `currentInput` and call `advanceStage()` at once — just as if you had typed the number and pressed #.

← [All solutions](../README.md) · [Project code](../../P05_Calculator)
