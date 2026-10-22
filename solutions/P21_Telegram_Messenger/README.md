# Solutions — Project 21: Telegram Messenger

Fill in your Wi-Fi details, BOT_TOKEN and MY_CHAT_ID in each sketch before uploading. Never share them. Each folder includes MultiTap.h.

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Challenge 1: A /count command

Full sketch: [`C1_Remote_Counter/C1_Remote_Counter.ino`](C1_Remote_Counter/C1_Remote_Counter.ino)

A counter `keyPresses` goes up for every key. A new `else if (text == "/count")` sends it back. ("Today" here means since the ESP32 was switched on.)

## Challenge 2: Doorbell on key C

Full sketch: [`C2_Doorbell/C2_Doorbell.ino`](C2_Doorbell/C2_Doorbell.ino)

Key C sends a fixed message to your own chat with `bot.sendMessage(MY_CHAT_ID, …)`. Put the device near the door, and your phone rings wherever you are.

## Challenge 3: Remote screen: /invert

Full sketch: [`C3_Remote_Invert/C3_Remote_Invert.ino`](C3_Remote_Invert/C3_Remote_Invert.ino)

The command flips a `bool` and calls `display.invertDisplay()` — the same function as in Project 20 — then replies to say what it did.

## Challenge 4: Family bot: two allowed chat IDs

Full sketch: [`C4_Family_Bot/C4_Family_Bot.ino`](C4_Family_Bot/C4_Family_Bot.ino)

An array `allowedIDs` holds every chat that may use the bot. A `for` loop checks if the sender is in the list; only then is the message accepted. Put a parent's real chat ID in place of "111111111" (find it the same way as yours).

## Challenge 5: Show the time each message arrived

Full sketch: [`C5_Message_Time/C5_Message_Time.ino`](C5_Message_Time/C5_Message_Time.ino)

Add `configTzTime()` from Project 18 in `setup()`. When a message arrives, `getLocalTime()` and `strftime(…, "%H:%M", …)` turn the time into text like 14:05, which is shown next to the sender's name.

← [All solutions](../README.md) · [Project code](../../P21_Telegram_Messenger)
