# Solutions — Project 22: QR Code Maker

Each folder includes MultiTap.h. Change the Wi-Fi name and password before you share a Wi-Fi QR code.

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Challenge 1: Phone number QR

Full sketch: [`C1_Contact_QR/C1_Contact_QR.ino`](C1_Contact_QR/C1_Contact_QR.ino)

Type a phone number and press A. The code holds `"tel:" + mtText`, so a phone that scans it opens the dialler with the number ready.

## Challenge 2: Big mode for short text

Full sketch: [`C2_Big_Mode/C2_Big_Mode.ino`](C2_Big_Mode/C2_Big_Mode.ino)

A version 1 code has 21 squares. 21 × 3 = 63 pixels, so it fits the 64-pixel screen height at 3 × 3 pixels per square. A local variable `scale` is 3 for 21 squares and 2 for the rest. Big squares are easier for old phone cameras.

## Challenge 3: QR from the Contact Book

Full sketch: [`C3_Contact_Book_QR/C3_Contact_Book_QR.ino`](C3_Contact_Book_QR/C3_Contact_Book_QR.ino)

Upload Project 15 first and save some contacts. This sketch uses the same EEPROM layout (10 bytes name + 15 bytes number per slot). Each press of A finds the next slot with a good number and shows its `tel:` code with the name as the label. Uploading a new sketch does not erase EEPROM, so the contacts are still there.

## Challenge 4: Website QR on key C

Full sketch: [`C4_Website_QR/C4_Website_QR.ino`](C4_Website_QR/C4_Website_QR.ino)

A fixed text `WEBSITE` is turned into a code when C is pressed. Version 3 holds about 50 letters, so keep the address short.

← [All solutions](../README.md) · [Project code](../../P22_QR_Code_Generator)
