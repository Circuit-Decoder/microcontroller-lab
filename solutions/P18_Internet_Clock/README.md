# Solutions — Project 18: Internet Clock

Remember to type your own Wi-Fi name and password in each sketch before uploading.

Try each challenge yourself first! Then compare with these answers. There is often more than one good solution.

## Challenge 1: Prayer and Iftar times

Full sketch: [`C1_Prayer_Times/C1_Prayer_Times.ino`](C1_Prayer_Times/C1_Prayer_Times.ino)

Three parallel arrays hold the names, hours and minutes. The times in the code are EXAMPLES — they change every day, so copy today's times for your city. The trick is to turn every time into "minutes since midnight" (hour × 60 + minute): then the next prayer is simply the first one with a bigger number, and the waiting time is a subtraction. `/ 60` gives the hours and `% 60` the minutes left over. Press C to see it.

## Challenge 2: World clock

Full sketch: [`C2_World_Clock/C2_World_Clock.ino`](C2_World_Clock/C2_World_Clock.ino)

`setenv("TZ", …)` and `tzset()` change the time zone while the program runs. Each city has its own rule; London and New York's rules also say when summer time starts and ends, so the clock stays right all year. Press C to change city.

## Challenge 3: Alarm

Full sketch: [`C3_Alarm/C3_Alarm.ino`](C3_Alarm/C3_Alarm.ino)

Once a second, `checkAlarm()` compares the hour and minute with the alarm time. During that minute, `display.invertDisplay()` swaps black and white on and off, so the screen flashes. Any key stops it until tomorrow.

## Challenge 4: Class timetable

Full sketch: [`C4_Class_Timetable/C4_Class_Timetable.ino`](C4_Class_Timetable/C4_Class_Timetable.ino)

Each period has a start time. Turn everything into minutes since midnight; the current period is the LAST one that has already started. The time to the next period is again a subtraction. Change the names and times to your own school's timetable, then press C.

← [All solutions](../README.md) · [Project code](../../P18_Internet_Clock)
