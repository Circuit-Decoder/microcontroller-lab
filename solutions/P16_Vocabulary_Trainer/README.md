# Solutions — Project 16: English Vocabulary Trainer

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Challenge 1: Show the cards in a random order

Full sketch: [`C1_Shuffle_Cards/C1_Shuffle_Cards.ino`](C1_Shuffle_Cards/C1_Shuffle_Cards.ino)

Don't move the cards — shuffle a list of card numbers, `order[]`, and show `cards[order[currentCard]]`. The shuffle goes from the last place down; each place swaps with a random place at or before it. This "Fisher–Yates" shuffle gives every order the same chance.

## Challenge 2: Word of the Day with an example

Full sketch: [`C2_Word_Of_The_Day/C2_Word_Of_The_Day.ino`](C2_Word_Of_The_Day/C2_Word_Of_The_Day.ino)

A new field `String example;` is added to the struct, so every card gets one more value at the end. At start-up, one random card is shown with its meaning and example, and `while (!keypad.getKey()) { }` waits for any key before the quiz starts.

## Challenge 3: Formula trainer

Full sketch: [`C3_Formula_Trainer/C3_Formula_Trainer.ino`](C3_Formula_Trainer/C3_Formula_Trainer.ino)

Only the data changes: the same struct now holds formulas like "Speed =" with the choices "d x t", "d / t" … The program itself is the same — that is the power of keeping data and code apart.

## Challenge 4: Spell it with MultiTap

Full sketch: [`C4_Spell_It/C4_Spell_It.ino`](C4_Spell_It/C4_Spell_It.ino)

Include `MultiTap.h` (it is in this folder). The screen shows the meaning; the player types the word. `mtUpdate(key)` must run in every loop, even without a key, so the `if (!key) return;` line is gone. `#` compares `mtText == cards[currentCard].word` — `==` compares whole Strings, letter by letter.

← [All solutions](../README.md) · [Project code](../../P16_Vocabulary_Trainer)
