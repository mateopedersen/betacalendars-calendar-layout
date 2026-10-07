# Architecture

The public API is a single C++17 header organized around three value domains:

1. `civil_date` and Gregorian arithmetic, which use no clock, timezone, or locale.
2. `month_topology` and `grid_cell`, which describe month shape and optional spillover dates.
3. `layout_spec` and `layout_metrics`, which map paper dimensions and reserved areas to physical cell geometry.

The library is header-only. The optional CLI and tests link the same exported interface target used by consumers. Input errors in ordinary layout/date validation are represented by `result<T, layout_error>`; programmer-readable enums describe modes and policies.
