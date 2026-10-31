# practice_3_countdown_with_while

**Task:** Countdown with while: rewrite E9.1's countdown with a while loop. Which version is clearer?

The `for` version is clearer for counting, because start, test and step sit together on one line: `for (int i = 10; i >= 1; i--)`. With `while`, the three parts are spread out, and forgetting `i--` gives a loop that never ends. Use `while` when you do not know in advance how many rounds you need (as in Practice 4).
