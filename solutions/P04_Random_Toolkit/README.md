# Solutions — Project 4: Random Toolkit

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Challenge 1: Roll 2 dice on key 2

Full sketch: [`C1_Two_Dice/C1_Two_Dice.ino`](C1_Two_Dice/C1_Two_Dice.ino)

Call `random(1, 7)` twice, once for each die, and print both plus their total `die1 + die2`. A new `else if (key == '2')` in `loop()` runs it.

## Challenge 2: Choose the password length (4–9, then D)

Full sketch: [`C2_Password_Length/C2_Password_Length.ino`](C2_Password_Length/C2_Password_Length.ino)

A variable `passwordLength` starts at 6. Keys 4 to 9 change it with `key - '0'` (the trick from Project 2). The `for` loop then runs `passwordLength` times instead of 6.

## Challenge 3: Random Name Picker

Full sketch: [`C3_Name_Picker/C3_Name_Picker.ino`](C3_Name_Picker/C3_Name_Picker.ino)

Store 10 names in a `String` array, then pick a random place with `random(0, 10)` — that gives 0 to 9, exactly the indexes of the array.

## Challenge 4: Maths Quiz mode

Full sketch: [`C4_Maths_Quiz/C4_Maths_Quiz.ino`](C4_Maths_Quiz/C4_Maths_Quiz.ino)

A variable `inQuiz` remembers whether the quiz is running. While it is, every key goes to `handleQuizKey()`: digits are collected in `answer`, `#` checks `answer.toInt()` against `num1 * num2`, and `*` leaves the quiz. `score` and `asked` keep the score.

← [All solutions](../README.md) · [Project code](../../P04_Random_Toolkit)
