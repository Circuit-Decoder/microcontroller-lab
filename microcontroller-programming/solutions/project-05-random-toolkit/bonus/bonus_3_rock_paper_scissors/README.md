# bonus_3_rock_paper_scissors

**Task:** Rock, paper, scissors: add a game on key #: you press 1, 2 or 3, the ESP32 picks one at random, and the screen says who won.

In the order Rock, Paper, Scissors, each move is beaten by the next one (and Scissors by Rock, wrapping round). So `(you + 1) % 3 == me` means the ESP32 wins. Writing out all nine cases with `if` works too. The `%` operator is explained in Chapter 12.
