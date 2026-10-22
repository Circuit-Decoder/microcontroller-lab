# Solutions — Project 8: Statistics Tool

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Challenge 1: Mode: the value that appears most often

Full sketch: [`C1_Mode/C1_Mode.ino`](C1_Mode/C1_Mode.ino)

After sorting, equal values sit next to each other: 45 72 72 72 81. `findMode()` counts each "run" of equal values and remembers the longest. If every run is 1 long, there is no mode.

## Challenge 2: Grade counter

Full sketch: [`C2_Grade_Counter/C2_Grade_Counter.ino`](C2_Grade_Counter/C2_Grade_Counter.ino)

Two parallel arrays hold the lowest mark of each grade (80, 70, 60, 50, 40, 33, 0) and its name (A+, A, A-, B, C, D, F). For each mark, an inner loop finds the first grade it reaches and `break`s. Press C on the results screen to see the totals. (These are the usual Bangladesh school grade limits.)

## Challenge 3: Pass rate

Full sketch: [`C3_Pass_Rate/C3_Pass_Rate.ino`](C3_Pass_Rate/C3_Pass_Rate.ino)

Count the marks that are 33 or above, then `passed * 100.0 / count`. The `100.0` (not 100) keeps the decimal part — the same trick as Project 5.

## Challenge 4: Sort high to low with C

Full sketch: [`C4_High_To_Low/C4_High_To_Low.ino`](C4_High_To_Low/C4_High_To_Low.ino)

No need to sort again! The array is already sorted low to high, so just walk it backwards: `for (int i = count - 1; i >= 0; i--)`. A `bool highToLow` remembers which way to show it, and C flips it.

← [All solutions](../README.md) · [Project code](../../P08_Statistics_Tool)
