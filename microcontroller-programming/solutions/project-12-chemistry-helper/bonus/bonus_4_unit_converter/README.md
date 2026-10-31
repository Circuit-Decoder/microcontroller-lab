# bonus_4_unit_converter

**Task:** Unit converter: use the same "lookup + formula" idea for km ↔ miles (× 0.621), kg ↔ pounds (× 2.205) and °C ↔ °F (× 9 ÷ 5 + 32).

Six conversions, one formula: `(x - before) * factor + after`. Distances and weights only need a factor; temperature also needs the 32 shift, before (F to C) or after (C to F). Adding a conversion means adding one entry to each list, not new code. Check: 100 C = 212.00 F, 10 km = 6.21 miles.
