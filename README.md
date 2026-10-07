# BetaCalendars Calendar Toolkit

[![Copr build status](https://copr.fedorainfracloud.org/coprs/mateopedersen/betacalendars-calendar-layout/package/betacalendars-calendar-layout/status_image/last_build.png)](https://copr.fedorainfracloud.org/coprs/mateopedersen/betacalendars-calendar-layout/package/betacalendars-calendar-layout/)

**BetaCalendars Calendar Toolkit** combines the dependency-free C++17 calendar-layout library with `betacal`, an offline command-line tool for Gregorian calendar grids, year-boundary inspection, blank planners, and print geometry. The project is maintained by [Beta Calendars](https://www.betacalendars.com/).

The engine works offline. It uses civil dates rather than timestamps, so results do not depend on locale, timezone, daylight-saving rules, network services, or website content.

## Design goals

- Validated civil dates for years 1–9999 and deterministic Gregorian weekday arithmetic.
- All seven week starts, natural four-to-six-row grids, and fixed 42-cell grids.
- Explicit adjacent-date or blank-cell policy.
- A4, A5, US Letter, US Legal, and custom page sizes in millimeters.
- Reusable undated 5×7 and 6×7 grids and custom row/column geometry.
- C++17 header-only core, no runtime third-party dependencies, exported CMake target.

## Installation

Build and install from a source checkout:

```sh
cmake -S . -B build -DCMAKE_INSTALL_PREFIX="$PWD/install" \
  -DBETACALENDARS_BUILD_TESTS=ON -DBETACALENDARS_BUILD_TOOLS=ON
cmake --build build
ctest --test-dir build --output-on-failure
cmake --install build
```

In a downstream CMake project:

```cmake
find_package(BetaCalendarsCalendarLayout CONFIG REQUIRED)
target_link_libraries(myapp PRIVATE BetaCalendars::CalendarLayout)
```

The CLI and tests are optional CMake components and are both enabled for the RPM build.

## Command-line toolkit

The `betacal` executable works offline and supports text, JSON, CSV, and SVG
where the output is meaningful. The original `betacal-layout` command remains
available for existing package users:

```sh
betacal version
betacal month --year 2027 --month 1 --week-start monday --grid natural
betacal month --year 2027 --month 12 --week-start sunday --grid fixed --format json
betacal month --year 2027 --month 1 --format svg --output january.svg
betacal year --year 2027 --format csv
betacal blank --rows 6 --paper a4 --orientation landscape --format svg
betacal paper --paper letter --orientation portrait --format json
betacal compare --rows 6 --format csv
betacal inspect --year 2026 --format json
betacal validate --from-year 1900 --to-year 2100
```

Month output includes the selected week origin and either the natural row
count or a fixed 42-cell grid. Paper sizes are A4 (210×297 mm), A5 (148×210
mm), US Letter (215.9×279.4 mm), and US Legal (215.9×355.6 mm); orientation
can be portrait or landscape. `--margin`, `--header`, `--weekday-header`,
`--notes`, and `--cell-padding` accept millimetres. Invalid or non-positive
printable layouts fail with a diagnostic instead of emitting unusable geometry.

## Quick start

```cpp
#include <betacalendars/calendar_layout.hpp>
using namespace betacalendars::calendar_layout;

const auto january = topology(2027, 1, week_start::monday);
// january.days == 31, january.natural_rows == 5
auto cells = month_grid(2027, 1, week_start::monday,
                        grid_mode::natural,
                        adjacent_policy::include_adjacent_dates);
```

`month_grid` returns row/column, weekday, relation to the requested month, and an optional civil date for each cell. With `empty_adjacent`, spillover positions have no date and are explicitly marked empty.

## Civil dates, not timestamps

`civil_date` is a validated date value, not a time-of-day. The weekday routine uses integer Gregorian arithmetic; callers should validate external values with `valid_date` before passing them to `weekday_of`.

## Seven week starts

`week_start` and `weekday` use Monday-based indices from 0 through 6. `relative_weekday_index` and `offset_from_week_start` provide the zero-based position within a chosen week.

## Natural and fixed grids

Natural grids use the minimum number of whole weeks required for the month. They can contain four, five, or six rows. Fixed grids always contain 42 cells. `month_topology` reports day count, first and last weekdays, leading and trailing positions, natural row count, and fixed capacity.

## Paper geometry

`measure` computes page, printable, grid, and cell dimensions, cell area, and interior writing area after a configurable per-side padding. Defaults are a 10 mm edge margin, 16 mm title band, 8 mm weekday header, and 2 mm cell padding. All returned physical values are millimeters or square millimeters. Invalid dimensions return a `layout_error` through the C++17 `result` type.

## Blank grids

`measure_blank` computes undated geometry directly. The CLI supports date-free
5×7 and 6×7 layouts. This is distinct from removing labels from a dated calendar.

## 2027 structural atlas

The computed month-by-month weekday and row-count table, with visual references, is in [docs/year-2027-atlas.md](docs/year-2027-atlas.md).

## 2026–2027 regression window

The four-month boundary fixture and its calculated grid behavior are documented in [docs/boundary-2026-2027.md](docs/boundary-2026-2027.md).

`inspect` reports the year length, leap-year status, the weekday transition from
December 31 to the following January 1, and a seven-day date window around that
boundary. It accepts text, JSON, and CSV output; the next year must be within the
supported 1–9999 civil-date range.

## Validation

Configure with `-DBETACALENDARS_BUILD_TESTS=ON`, then build and run CTest. The
test program checks Gregorian year lengths and month topology for every year
from 1 through 9999, plus materialized natural and fixed grids for every month
and week start from 1900 through 2100. It checks date uniqueness/completeness,
leap-year cases, paper dimensions, known weekday cases, the 2027 365-day atlas,
and the 2026–2027 year boundary. When tools are enabled, CTest also runs CLI
smoke checks and the exhaustive validation command.

## Fedora RPM packages

The source tree includes a Fedora Copr spec that builds the runtime CLI,
development headers/CMake package, and offline documentation. The public
repository and current DNF enable/install commands are listed on the Copr
project page. The RPM is built from the tagged source release and runs CTest in
the Fedora builder.

## Human-readable calendar references

The C++ library calculates calendar topology independently. These Beta Calendars pages are human-readable printable references useful for visual comparisons; they are not data sources for the library.

- [Blank Calendar](https://www.betacalendars.com/blank-calendar)
- [January](https://www.betacalendars.com/january-calendar.html)
- [February](https://www.betacalendars.com/february-calendar.html)
- [March](https://www.betacalendars.com/march-calendar.html)
- [April](https://www.betacalendars.com/april-calendar.html)
- [May](https://www.betacalendars.com/may-calendar.html)
- [June](https://www.betacalendars.com/june-calendar.html)
- [July](https://www.betacalendars.com/july-calendar.html)
- [August](https://www.betacalendars.com/august-calendar.html)
- [September](https://www.betacalendars.com/september-calendar.html)
- [October](https://www.betacalendars.com/october-calendar.html)
- [November](https://www.betacalendars.com/november-calendar.html)
- [December](https://www.betacalendars.com/december-calendar.html)

## License

MIT. See [LICENSE](LICENSE).
