# Solutions — Project 23: Bluetooth Presentation Remote

Each folder includes BluetoothKeys.h (and C3 also MultiTap.h).

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Challenge 1: Time warning

Full sketch: [`C1_Time_Warning/C1_Time_Warning.ino`](C1_Time_Warning/C1_Time_Warning.ino)

`TALK_SECONDS` sets the talk length. When the timer passes it, `(millis() / 500) % 2` switches between 0 and 1 every half second, and `display.invertDisplay()` flashes the screen. It stops when D ends the slideshow.

## Challenge 2: Page Up and Page Down on keys 2 and 8

Full sketch: [`C2_Page_Up_Down/C2_Page_Up_Down.ino`](C2_Page_Up_Down/C2_Page_Up_Down.ino)

Two new HID codes: 0x4B (Page Up) and 0x4E (Page Down). They also work in web pages and Word.

## Challenge 3: Typing machine

Full sketch: [`C3_Typing_Machine/C3_Typing_Machine.ino`](C3_Typing_Machine/C3_Typing_Machine.ino)

A new sketch that joins Project 13 and Project 23. Type with MultiTap, then # sends the words to the laptop one key press at a time. `sendChar()` turns each letter into an HID code: capitals use the Shift modifier, a space is 0x2C. A = send + Enter, B = Backspace. If the laptop was paired with the remote before, remove it from the Bluetooth list and pair again with "CD Keyboard".

## Challenge 4: Laser pointer on key 0

Full sketch: [`C4_Laser_Pointer/C4_Laser_Pointer.ino`](C4_Laser_Pointer/C4_Laser_Pointer.ino)

`btPress(KEY_A_LETTER + 11, MOD_CTRL)` is the same as `btPress(0x0F, 0x01)`: L is the 12th letter, so 0x04 + 11 = 0x0F, and 0x01 is the Ctrl modifier. Press 0 again to turn the laser off.

← [All solutions](../README.md) · [Project code](../../P23_Presentation_Remote)
