# Solutions — Project 11: Graph Plotter

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Challenge 1: Two graphs at once — simultaneous equations

Full sketch: [`C1_Two_Lines/C1_Two_Lines.ino`](C1_Two_Lines/C1_Two_Lines.ino)

A 4th graph type (key D) asks for m1, c1, m2 and c2. The first line is drawn as usual; `drawSecondLine()` draws the second and finds the crossing point: m1x + c1 = m2x + c2 gives x = (c2 − c1) ÷ (m1 − m2). For y = 2x + 1 and y = −x + 4 the lines cross at (1, 3). If the slopes are equal, the lines are parallel and never cross.

## Challenge 2: A readable label

Full sketch: [`C2_Readable_Label/C2_Readable_Label.ino`](C2_Readable_Label/C2_Readable_Label.ino)

`display.setTextColor(SSD1306_WHITE, SSD1306_BLACK)` means "white letters on a black box", so the curve no longer runs through the text. Set it back to plain white afterwards.

## Challenge 3: Move the view with 4, 6, 2 and 8

Full sketch: [`C3_Move_The_View/C3_Move_The_View.ino`](C3_Move_The_View/C3_Move_The_View.ino)

Two new variables, `viewX` and `viewY`, say which maths point sits in the centre of the screen. Both conversion formulas subtract them: `64 + (x - viewX) * scale`. Each key press moves the view 16 pixels (16 ÷ scale units). The axes are drawn where x = 0 and y = 0 are now, so they move too.

## Challenge 4: Cosine, cubic and a / x

Full sketch: [`C4_Cosine_Cubic_Reciprocal/C4_Cosine_Cubic_Reciprocal.ino`](C4_Cosine_Cubic_Reciprocal/C4_Cosine_Cubic_Reciprocal.ino)

Three new types: D = a cos(bx), 1 = ax³ and 2 = a ÷ x. For a ÷ x, dividing by 0 has no answer, so `f()` returns `NAN` ("not a number") at x = 0. The drawing loop does not join points across that gap — otherwise you would see a false vertical line where the curve jumps from far below to far above.

← [All solutions](../README.md) · [Project code](../../P11_Graph_Plotter)
