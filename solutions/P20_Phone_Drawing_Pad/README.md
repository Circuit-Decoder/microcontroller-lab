# Solutions — Project 20: Phone Drawing Pad

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Challenge 1: Eraser

Full sketch: [`C1_Eraser/C1_Eraser.ino`](C1_Eraser/C1_Eraser.ino)

The page gets an Eraser button. While it is ON, the page sends `E` instead of `M` and draws in black on the phone too. On the ESP32, `E` draws a line in `SSD1306_BLACK` — black pixels on a black screen look like erasing.

## Challenge 2: Thick pen

Full sketch: [`C2_Thick_Pen/C2_Thick_Pen.ino`](C2_Thick_Pen/C2_Thick_Pen.ino)

`fillCircle(x, y, 2, …)` draws a dot 5 pixels wide. For a line, `thickLine()` walks from the last point to the new one and stamps a small circle at every step — the same way the phone page draws its line pixel by pixel.

## Challenge 3: Save and load a drawing

Full sketch: [`C3_Save_Drawing/C3_Save_Drawing.ino`](C3_Save_Drawing/C3_Save_Drawing.ino)

The screen has 128 × 64 = 8192 pixels; each byte holds 8 of them, so the whole picture is 1024 bytes. `display.getBuffer()` gives the address of those bytes in memory. A copies them into EEPROM, B copies them back and redraws. The drawing now survives a power cut!

## Challenge 4: Two artists at once

Full sketch: [`C4_Two_Artists/C4_Two_Artists.ino`](C4_Two_Artists/C4_Two_Artists.ino)

What goes wrong with one `lastX`/`lastY`: the messages of the two phones arrive mixed together, so a line from phone 1 starts at the last point of phone 2 — you see long wrong lines jumping between the drawings. The fix: the library tells us WHICH phone sent each message (`client`), so we keep one last point per phone in two arrays. The library allows up to 5 phones at once.

← [All solutions](../README.md) · [Project code](../../P20_Phone_Drawing_Pad)
