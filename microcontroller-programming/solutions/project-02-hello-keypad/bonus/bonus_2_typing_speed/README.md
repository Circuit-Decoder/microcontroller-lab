# bonus_2_typing_speed

**Task:** Typing speed: count presses for a fixed time, then show the result. Come back to this after Chapter 18, which teaches millis().

Presses in 10 seconds × 6 = presses per minute. `getKey()` never waits, so `loop()` keeps checking the clock while you type.
