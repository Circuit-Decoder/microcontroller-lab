# practice_1_tidy_project_03

**Task:** Tidy Project 03: in your PIN lock, move the code that draws the "Enter PIN" screen into void showEntryScreen(), and call it from every place that drew it. Check that the lock still passes its Expected Behaviour table.

Because `showEntryScreen()` is called after every key, the `redraw` flag from the original is no longer needed. The two result screens became a second helper, `showMessage()`.
