# Solutions — Project 15: Contact Book

Each solution folder includes MultiTap.h from Project 13.

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Challenge 1: List all contacts with C

Full sketch: [`C1_List_All/C1_List_All.ino`](C1_List_All/C1_List_All.ino)

`showNextContact()` looks for the next USED slot after the one on screen. `(listSlot + step) % MAX_CONTACTS` wraps around to slot 0 after the last slot — the remainder trick from Project 6.

## Challenge 2: Delete a contact with *

Full sketch: [`C2_Delete_Contact/C2_Delete_Contact.ino`](C2_Delete_Contact/C2_Delete_Contact.ino)

Remember which slot the search found (`foundSlot`). To delete, write the end marker `'\0'` in the first box of the name — and also of the number, because `slotIsUsed()` checks the number. Then `findEmptySlot()` makes the free slot available again.

## Challenge 3: No duplicates

Full sketch: [`C3_No_Duplicates/C3_No_Duplicates.ino`](C3_No_Duplicates/C3_No_Duplicates.ino)

Before saving, `findSlotByName()` checks if the name already exists. If yes, the new number is written into that same slot, so DAD is updated instead of being saved twice.

## Challenge 4: Search by number

Full sketch: [`C4_Search_By_Number/C4_Search_By_Number.ino`](C4_Search_By_Number/C4_Search_By_Number.ino)

Exactly the same linear search as `findContact()`, but it compares `readNumberFromSlot(slot)` with the typed number and gives back the name.

← [All solutions](../README.md) · [Project code](../../P15_Contact_Book)
