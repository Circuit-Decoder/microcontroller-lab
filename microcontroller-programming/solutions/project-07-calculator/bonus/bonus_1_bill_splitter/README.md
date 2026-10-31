# bonus_1_bill_splitter

**Task:** Bill splitter: a second mode (press # in stage 1) that asks for a bill, a tip percentage and the number of people, then shows each person's share. For example, 1,200 taka with a 10% tip shared by 4 people is 330 taka each.

Test: 1200 #, 10 #, 4 # gives 330.00. The splitter is a second, smaller stage machine that takes over the keys while `billStage` is not 0. People = 0 is refused, so there is no division by zero.
