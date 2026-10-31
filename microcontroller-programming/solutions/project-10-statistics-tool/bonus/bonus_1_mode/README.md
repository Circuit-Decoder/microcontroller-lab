# bonus_1_mode

**Task:** Mode: also show the value that appears most often.

Because the values are already sorted, the same values are neighbours, so one pass counting "runs" finds the mode. If no value appears twice there is no mode. If two values tie, the smaller one is shown. (`int &bestRun` lets the function hand back a second answer; if that is new to you, a global variable works too.)
