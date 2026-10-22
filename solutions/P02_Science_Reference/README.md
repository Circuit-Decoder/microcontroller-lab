# Solutions — Project 2: Science Reference

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Challenge 1: Part A: 3 more constants on keys A, B and C

Full sketch: [`C1_Three_More_Constants/C1_Three_More_Constants.ino`](C1_Three_More_Constants/C1_Three_More_Constants.ino)

Make all three arrays one size bigger for each new constant ([10] → [13]) and add the new values at the end, at places 10, 11 and 12. The trick `key - '0'` only works for digits, so the letters get their own checks: `else if (key == 'A') showConstant(10);` and so on.

## Challenge 2: Part B: elements 21 to 30

Full sketch: [`C2_Elements_To_30/C2_Elements_To_30.ino`](C2_Elements_To_30/C2_Elements_To_30.ino)

Make both arrays [31] (places 0 to 30), add Scandium to Zinc at the end, and change the range check to `number <= 30`.

## Challenge 3: Part B: a third array for atomic mass

Full sketch: [`C3_Atomic_Mass/C3_Atomic_Mass.ino`](C3_Atomic_Mass/C3_Atomic_Mass.ino)

Add a third parallel array, `double atomicMasses[21]`, in the same order. We use `double` because chlorine is 35.5. `display.println(atomicMasses[number], 1);` shows one decimal place.

## Challenge 4: Big challenge: both tools in one program

Full sketch: [`C4_Both_Tools/C4_Both_Tools.ino`](C4_Both_Tools/C4_Both_Tools.ino)

Use a variable `tool` that remembers which tool is running (0 = menu, 1 = constants, 2 = periodic table). In `loop()`, an `if / else if` chain sends each key to the right tool. D always goes back to the menu. All the arrays and screen functions from Part A and Part B are simply copied into one sketch.

← [All solutions](../README.md) · Project code: [Part A](../../P02_Science_Reference_PartA), [Part B](../../P02_Science_Reference_PartB)
