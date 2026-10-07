# Calendar topology

`topology(year, month, start)` reports the first/last weekday, leading cells, minimum whole-week row count, trailing cells, and fixed-grid capacity. `month_grid` materializes either the minimal grid or six full rows. Adjacent cells can carry their actual neighboring Gregorian dates or remain empty.

`topology_signature` is a compact representation used by this library. It is not a universal calendar standard.
