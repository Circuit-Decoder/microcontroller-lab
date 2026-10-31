# bonus_4_delete_key

**Task:** Delete: make one key delete the last digit typed. Which key would you choose, and what has to move?

All 16 keys already have a job, so something has to share. Here `*` deletes one digit, and only when nothing is left to delete does it clear the whole sum. "Clear" moved from "every press of *" to "* on an empty number". `String.remove(i)` cuts everything from position `i` onwards.
