# practice_4_bounce_hunt

**Task:** Bounce hunt: in E2.2, use delay(1) instead of delay(200). Press the button once, slowly, and scroll back through the readings. Can you find a place where 1 and 0 swap back and forth before settling? (You may not catch one every time.)

**What to look for:** untick *Autoscroll* in the Serial Monitor after one slow press, then scroll to where the 1s change to 0s. Sometimes you find a short burst like `1 1 0 1 0 0 1 0 0 0 ...`: the metal contacts bounce for a few milliseconds before they settle. With `delay(200)` you almost never see it, because you only look five times a second. Chapter 2's edge detection with a short delay is the usual cure.
