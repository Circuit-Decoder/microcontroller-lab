# Solutions — Project 7: Quadratic Equation Solver

Challenges 3, 4 and 5 are new programs that reuse the number-typing part of the Quadratic Solver: B = minus, C = decimal point, D = delete, # = OK.

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Challenge 1: Show the equation on the result screen

Full sketch: [`C1_Show_Equation/C1_Show_Equation.ino`](C1_Show_Equation/C1_Show_Equation.ino)

A small helper `num()` turns a number into text without useless decimals (1 instead of 1.00). Then one line joins the parts: `num(a) + "x^2 + " + num(b) + "x + " + num(c) + " = 0"`. One empty line was removed so everything still fits on the screen.

## Challenge 2: Complex roots p ± qi (HSC)

Full sketch: [`C2_Complex_Roots/C2_Complex_Roots.ino`](C2_Complex_Roots/C2_Complex_Roots.ino)

When D < 0, `-D` is positive, so `sqrt(-D)` works. p = −b ÷ 2a is the real part and q = √(−D) ÷ 2a is the imaginary part. For x² + 2x + 5 = 0 you get −1 + 2i and −1 − 2i.

## Challenge 3: Electricity bill with slabs

Full sketch: [`C3_Electricity_Bill/C3_Electricity_Bill.ino`](C3_Electricity_Bill/C3_Electricity_Bill.ino)

The rates in the code are EXAMPLES — copy the real ones from your bill. A `for` loop walks through the slabs from the lowest up. In each slab, `min(units, limit) - previousLimit` is how many units fall inside that slab, and they are charged at that slab's rate. The real bill also adds a demand charge and VAT. (We used two parallel arrays, just like Project 2.)

## Challenge 4: Savings growth with compound interest

Full sketch: [`C4_Savings_Growth/C4_Savings_Growth.ino`](C4_Savings_Growth/C4_Savings_Growth.ino)

Type the money, the rate and the years, pressing # after each. `pow(1 + r / 100.0, n)` means (1 + r/100) to the power n. For 10,000 taka at 6% for 5 years you get 13,382.26 taka.

## Challenge 5: BMI calculator

Full sketch: [`C5_BMI_Calculator/C5_BMI_Calculator.ino`](C5_BMI_Calculator/C5_BMI_Calculator.ino)

Two stages: height in cm, then weight in kg (C is the decimal point). Divide the height by 100.0 to get metres, then BMI = weight ÷ (height × height). Remember: the usual BMI categories are only for adults.

← [All solutions](../README.md) · [Project code](../../P07_Quadratic_Solver)
