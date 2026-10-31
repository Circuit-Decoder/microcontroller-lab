# bonus_2_signal_log

**Task:** Signal log: print Button A's reading on the Serial Monitor while you signal, so the computer can record your message. Does adding a delay() to slow the printing spoil the lamp?

**Answer:** a short delay (10–50 ms) is fine: the lamp still follows your finger, and the log shows each dot and dash as a run of 0s. A long delay such as `delay(500)` spoils the lamp: a quick dot can start and end between two looks, so the lamp misses it or stays on too long. Long pauses inside `loop()` make a device deaf and blind for that time (Chapter 18 fixes this with `millis()`).
