# Solutions — Project 22: QR Code Maker

Each folder includes MultiTap.h. Change the Wi-Fi name and password before you share a Wi-Fi QR code.

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Challenge 1: Phone number QR

Full sketch: [`C1_Contact_QR/C1_Contact_QR.ino`](C1_Contact_QR/C1_Contact_QR.ino)

Type a phone number and press A. The code holds `"tel:" + mtText`, so a phone that scans it opens the dialler with the number ready.

## Challenge 2: Fit checker: OK or TOO LONG

Full sketch: [`C2_Fit_Checker/C2_Fit_Checker.ino`](C2_Fit_Checker/C2_Fit_Checker.ino)

Count the letters typed so far (plus the one being chosen) and show "OK - fits" for 17 or fewer, "TOO LONG" for more. 17 letters always fit in version 1, which sits completely below the yellow strip and scans well. (Only capitals, digits and spaces? Then up to 25 still fit — the counter plays it safe.)

## Challenge 3: QR from the Contact Book

Full sketch: [`C3_Contact_Book_QR/C3_Contact_Book_QR.ino`](C3_Contact_Book_QR/C3_Contact_Book_QR.ino)

Upload Project 15 first and save some contacts. This sketch uses the same EEPROM layout (10 bytes name + 15 bytes number per slot). Each press of A finds the next slot with a good number and shows its `tel:` code with the name as the label. Uploading a new sketch does not erase EEPROM, so the contacts are still there.

## Challenge 4: Website QR on key C

Full sketch: [`C4_Website_QR/C4_Website_QR.ino`](C4_Website_QR/C4_Website_QR.ino)

A fixed text `WEBSITE` is turned into a code when C is pressed. Keep the address to 17 letters or fewer (like https://nasa.gov), so it is a version 1 code below the yellow strip.

← [All solutions](../README.md) · [Project code](../../P22_QR_Code_Generator)
