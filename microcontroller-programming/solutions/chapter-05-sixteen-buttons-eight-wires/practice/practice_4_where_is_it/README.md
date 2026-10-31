# practice_4_where_is_it

**Task:** Where is it? For each key, write down its row and column on paper. Then check three of them with E5.1 or your answer to Practice 1.

No sketch needed: this is a paper task.

| Key | Row | Column |   | Key | Row | Column |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | 1 | 1 | | 3 | 1 | 3 |
| 2 | 1 | 2 | | A | 1 | 4 |
| 4 | 2 | 1 | | 6 | 2 | 3 |
| 5 | 2 | 2 | | B | 2 | 4 |
| 7 | 3 | 1 | | 9 | 3 | 3 |
| 8 | 3 | 2 | | C | 3 | 4 |
| * | 4 | 1 | | # | 4 | 3 |
| 0 | 4 | 2 | | D | 4 | 4 |

Rows are keypad pins 1–4 (GPIO 13, 12, 14, 27), columns are pins 5–8 (GPIO 26, 25, 33, 32). To check with E5.1 (column 1 only): pressing 1, 4, 7 or * makes row 1, 2, 3 or 4 read 0. With Practice 1's two-column scan you can check 2, 5, 8 and 0 too.
