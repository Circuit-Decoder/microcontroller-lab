# Solutions — Project 10: Chemistry Helper

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Challenge 1: Add HNO₃, Ca(OH)₂ and MgO

Full sketch: [`C1_More_Compounds/C1_More_Compounds.ino`](C1_More_Compounds/C1_More_Compounds.ino)

Make the compound arrays 12 rows long (a constant `NUM_COMPOUNDS` makes this easy) and add three rows at the end. Ca(OH)₂ has 1 Ca, 2 O and 2 H. There are only 9 number keys, so `#` switches to a second page, and on page 2 the key number is `page * 9 + (key - '1')`. Answers: HNO₃ = 63, Ca(OH)₂ = 74, MgO = 40 g/mol.

## Challenge 2: Exact masses with 3 decimals

Full sketch: [`C2_Exact_Masses/C2_Exact_Masses.ino`](C2_Exact_Masses/C2_Exact_Masses.ino)

Replace the rounded masses with exact ones and print with `display.print(v, 3)`. Water becomes 18.015 g/mol instead of 18, and table salt 58.440 instead of 58.5. The answers change only a little — that is why school books use rounded numbers.

## Challenge 3: Percentage composition

Full sketch: [`C3_Percentage_Composition/C3_Percentage_Composition.ino`](C3_Percentage_Composition/C3_Percentage_Composition.ino)

First add up the total mass. Then each element's share is `part / total * 100`. On a compound screen, press # to see it. Water: hydrogen 11.1%, oxygen 88.9%.

## Challenge 4: Unit converter

Full sketch: [`C4_Unit_Converter/C4_Unit_Converter.ino`](C4_Unit_Converter/C4_Unit_Converter.ino)

A new program with the same "lookup + formula" idea: parallel arrays hold the unit names and the multiply factors (0.621 for km → miles, and 1 ÷ 0.621 for the other way). Temperature cannot be done by multiplying alone, so `convert()` has a special formula for it: °F = °C × 9 ÷ 5 + 32.

← [All solutions](../README.md) · [Project code](../../P10_Chemistry_Helper)
