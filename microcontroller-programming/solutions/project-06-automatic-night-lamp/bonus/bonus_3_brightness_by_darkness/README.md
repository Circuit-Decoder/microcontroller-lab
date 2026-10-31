# bonus_3_brightness_by_darkness

**Task:** Brightness by darkness: in AUTO, make the lamp dimmer at dusk and brighter when it's completely dark.

`map()` with a reversed input range (onLevel down to 0) makes darker readings give bigger brightness. Between the on and off levels the reading can be above `onLevel`, so `constrain()` keeps the lamp at least at 60.
