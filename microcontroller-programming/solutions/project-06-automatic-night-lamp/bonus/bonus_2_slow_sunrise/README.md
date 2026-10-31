# bonus_2_slow_sunrise

**Task:** Slow sunrise: make the lamp take 10 seconds to fade off in the morning, but only half a second to come on.

Fading in: 255 / 10 = 26 loops x 20 ms, about half a second. Fading out: 255 steps, one every second loop (40 ms), about 10 seconds. `min()` stops the fade-in overshooting 255. (The screen drawing also takes a few ms per loop, so real times are a little longer.)
