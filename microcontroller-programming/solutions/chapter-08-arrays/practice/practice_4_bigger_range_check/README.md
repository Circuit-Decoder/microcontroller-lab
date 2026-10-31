# practice_4_bigger_range_check

**Task:** Bigger range check: add 3 more prices to E8.4 by changing only SIZE and the list. Check that 7 works and 8 doesn't.

Item 7 is the last box (index `SIZE - 1`). Item 8 fails the `item < SIZE` check, so it says "No such item". The check never needed changing, because it uses SIZE.
