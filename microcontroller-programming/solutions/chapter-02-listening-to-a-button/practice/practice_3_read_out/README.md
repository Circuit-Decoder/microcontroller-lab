# practice_3_read_out

**Task:** Read-out: add Serial.begin(115200); and a Serial.println(digitalRead(BUTTON_B)); line to E2.5, with a delay(100). Watch Button B's readings while the LEDs work. Does the delay make the LEDs feel slower to respond?

**Answer:** yes, a little. `loop()` now pauses for 100 ms each time, so a press can take up to a tenth of a second to light an LED. Most people barely notice 100 ms, but try `delay(1000)` and the lamps feel clearly slow. Chapter 18 shows how to do two things at once without `delay()`.
