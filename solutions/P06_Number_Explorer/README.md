# Solutions — Project 6: Number Explorer

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Challenge 1: All prime numbers from 1 to 100

Full sketch: [`C1_Prime_List/C1_Prime_List.ino`](C1_Prime_List/C1_Prime_List.ino)

A `for` loop goes from 2 to 100. For each number, `smallestFactor(n) == 0` means "no factor found", so the number is prime and gets printed. There are 25 of them. Press # without typing a number to see the list.

## Challenge 2: Prime factorisation, like 360 = 2 x 2 x 2 x 3 x 3 x 5

Full sketch: [`C2_Prime_Factorisation/C2_Prime_Factorisation.ino`](C2_Prime_Factorisation/C2_Prime_Factorisation.ino)

Keep dividing by the smallest factor: 360 → 2, then 180 → 2, 90 → 2, 45 → 3, 15 → 3, and 5 is prime. A `while (n > 1)` loop does exactly this. Type a number and press #.

## Challenge 3: Faster factors

Full sketch: [`C3_Faster_Factors/C3_Faster_Factors.ino`](C3_Faster_Factors/C3_Faster_Factors.ino)

Loop only while `d * d <= n`. Each time `d` divides `n`, you get two factors at once: `d` and `n / d`. The small ones are added at the end of one list, the big ones at the front of another, so the final list stays in order. Now even 9-digit numbers are fast.

## Challenge 4: Show the next page with A

Full sketch: [`C4_Factor_Pages/C4_Factor_Pages.ino`](C4_Factor_Pages/C4_Factor_Pages.ino)

The whole result is kept in `factorList`. `substring(start, start + 105)` cuts out one page of 5 lines. A new stage, 4, means "reading factors": A shows the next page (and goes back to page 1 at the end); any other key starts a new number.

← [All solutions](../README.md) · [Project code](../../P06_Number_Explorer)
