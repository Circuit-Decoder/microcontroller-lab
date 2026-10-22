# Solutions — Project 19: Phone Message Board

Each solution folder includes MultiTap.h from Project 13 (it is used for typing replies).

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Challenge 1: Classroom notice board with a password

Full sketch: [`C1_Teacher_Password/C1_Teacher_Password.ino`](C1_Teacher_Password/C1_Teacher_Password.ino)

The page gets a second box, `<input name="pw" type="password">` (type="password" shows dots). `handleSend()` reads it with `server.arg("pw")`. If it is wrong, the ESP32 answers with code 403 ("not allowed") and does not show the message. Change `TEACHER_PASSWORD` before you use it in class!

## Challenge 2: Short messages in big letters

Full sketch: [`C2_Big_Letters/C2_Big_Letters.ino`](C2_Big_Letters/C2_Big_Letters.ino)

Before printing the message, check `lastMessage.length() < 10`. If it is short, use text size 2; then go back to size 1 for the rest of the screen.

## Challenge 3: Message history with B

Full sketch: [`C3_Message_History/C3_Message_History.ino`](C3_Message_History/C3_Message_History.ino)

An array of 3 Strings keeps the newest message at `history[0]`. When a new message arrives, the older ones move down one place — from the bottom up, so nothing is overwritten too early (the same idea as the leaderboard in Project 17). B shows the next older message, and wraps back to the newest.

## Challenge 4: Style it

Full sketch: [`C4_Style_It/C4_Style_It.ino`](C4_Style_It/C4_Style_It.ino)

Only the `<style>` part changed: a dark background, green text, rounded borders and a bold button. Try your own colours! Colours like `#10151c` are written in hex — Project 9 again: each pair of hex digits is red, green and blue.

← [All solutions](../README.md) · [Project code](../../P19_Phone_Message_Board)
