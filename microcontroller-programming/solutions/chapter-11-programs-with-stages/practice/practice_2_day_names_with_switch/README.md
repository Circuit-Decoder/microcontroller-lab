# practice_2_day_names_with_switch

**Task:** Day names with switch: rewrite E8.2 using switch instead of an array. Which version is shorter?

Here both are about the same length, but the array wins as the list grows: the array version needs one `println(days[index])`, while switch needs a new line for every item. switch is better when each case does *different* work, not just picks a different word.
