# bonus_3_red_alert

**Task:** Red alert: make Button B light both the red lamp and the built-in LED. Keep the built-in LED following Button A as well. Now hold only Button B: why does the built-in LED look dimmer than when you hold A? (Chapter 6 shows how to fix it with "or".)

**Why is it dimmer with only Button B held?** Every time round `loop()`, the first line for the built-in LED switches it OFF (A is not pressed), and a moment later the second line switches it ON (B is pressed). So the LED is off for part of every loop: it flickers far too fast to see, and looks dimmer. With only A held it is the other way round: on, then off for most of the loop... and it also looks dimmer. Chapter 6 fixes this with one line: `digitalWrite(BUILTIN_LED, aPressed || bPressed);`.
