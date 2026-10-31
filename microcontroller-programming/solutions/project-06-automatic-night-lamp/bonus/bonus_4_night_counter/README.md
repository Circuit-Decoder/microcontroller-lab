# bonus_4_night_counter

**Task:** Night counter: count how many times the lamp has come on since the ESP32 started, and show it.

The count goes up only where `lampOn` changes from false to true, so a lamp that stays on all night counts once. Thanks to the hysteresis gap, a flickering reading near the level does not add extra counts. The count resets when the ESP32 restarts; Chapter 21 shows how to keep it.
