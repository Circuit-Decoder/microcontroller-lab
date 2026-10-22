# Solutions — Project 14: Secret Diary

Each solution folder includes MultiTap.h from Project 13.

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Challenge 1: Change the PIN (and save it)

Full sketch: [`C1_Change_PIN/C1_Change_PIN.ino`](C1_Change_PIN/C1_Change_PIN.ino)

Key 1 in the menu opens a "New PIN" screen. The new PIN is saved in boxes 92–99 with the same end marker idea as the note. At start-up, `loadPIN()` reads it back — but only digits count, so an empty EEPROM (255) or old data keeps the default 1234.

## Challenge 2: Delete the saved note

Full sketch: [`C2_Delete_Note/C2_Delete_Note.ino`](C2_Delete_Note/C2_Delete_Note.ino)

Writing the end marker `'\0'` into box 0 is enough: the reading loop stops at once, so the note is empty. You don't need to clear all 90 boxes.

## Challenge 3: A lockout that survives a restart

Full sketch: [`C3_Saved_Lockout/C3_Saved_Lockout.ino`](C3_Saved_Lockout/C3_Saved_Lockout.ino)

The number of wrong tries is saved in box 91. After 3, `waitLocked()` counts down 60 seconds. If someone pulls the plug, `setup()` reads the saved counter and starts the wait again — so restarting does not unlock the diary.

## Challenge 4: Three diary pages

Full sketch: [`C4_Three_Pages/C4_Three_Pages.ino`](C4_Three_Pages/C4_Three_Pages.ino)

Reserve 300 boxes and give each page its own 100: page 0 uses boxes 0–99, page 1 uses 100–199, page 2 uses 200–299. Every read and write just adds `page * PAGE_SIZE`. Keys 1, 2, 3 choose the page.

← [All solutions](../README.md) · [Project code](../../P14_Secret_Diary)
