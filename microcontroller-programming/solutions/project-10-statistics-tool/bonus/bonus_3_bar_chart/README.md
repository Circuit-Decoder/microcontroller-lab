# bonus_3_bar_chart

**Task:** Bar chart: draw a small bar for each value on the OLED, its height scaled with map().

`map()` works with whole numbers, so the values are multiplied by 100 first to keep two decimals of detail. Negative values are drawn as 1-pixel stubs. The one-pixel gap (`barWidth - 1`) keeps the bars apart.
