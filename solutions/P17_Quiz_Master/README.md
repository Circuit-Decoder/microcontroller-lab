# Solutions — Project 17: Quiz Master

Each solution folder includes MultiTap.h from Project 13.

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Challenge 1: Pick 5 random questions out of 10

Full sketch: [`C1_Random_Questions/C1_Random_Questions.ino`](C1_Random_Questions/C1_Random_Questions.ino)

There are now 10 questions per subject. Instead of taking the next one, the code picks a random question with `random(0, TOTAL_QUESTIONS)`, and keeps trying (a do-while loop again) until it finds one of the right subject that is not `usedQuestion[]` yet. The used list is cleared at the start of each round.

## Challenge 2: 10 seconds per question

Full sketch: [`C2_Ten_Second_Timer/C2_Ten_Second_Timer.ino`](C2_Ten_Second_Timer/C2_Ten_Second_Timer.ino)

`questionStart = millis()` when a question appears. Above `if (!key) return;` the code checks the time in every loop: after 10 seconds the answer counts as wrong and "Time up!" is shown. The seconds left appear in the top line, redrawn once a second.

## Challenge 3: Typed answers for English

Full sketch: [`C3_Typed_English_Answers/C3_Typed_English_Answers.ino`](C3_Typed_English_Answers/C3_Typed_English_Answers.ino)

For category 3, the answers are not shown. The player types with MultiTap, and # compares the text with the correct answer. The keyboard types only CAPITALS, so `right.toUpperCase()` turns "Went" into "WENT" before comparing.

## Challenge 4: A leaderboard for each subject

Full sketch: [`C4_Board_Per_Subject/C4_Board_Per_Subject.ino`](C4_Board_Per_Subject/C4_Board_Per_Subject.ino)

36 EEPROM boxes: subject 1 uses boxes 0–11, subject 2 uses 12–23, subject 3 uses 24–35. Every read and write adds `(category - 1) * 12`. Starting a round loads that subject's board. On the leaderboard screen, keys 1, 2 and 3 switch between subjects.

← [All solutions](../README.md) · [Project code](../../P17_Quiz_Master)
