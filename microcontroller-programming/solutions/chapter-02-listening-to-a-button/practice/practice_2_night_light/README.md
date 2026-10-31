# practice_2_night_light

**Task:** Night light: in E2.5, make the red LED shine when Button B is not pressed, and go dark while you hold it.

The whole trick is removing one `!`. With `INPUT_PULLUP` the reading is already HIGH when the button is not pressed.
