# Installation

Enable this Copr repository and install the runtime toolkit:

```sh
sudo dnf copr enable mateopedersen/betacalendars-calendar-layout
sudo dnf install betacalendars-calendar-layout
```

For C++ development headers and CMake package integration:

```sh
sudo dnf install betacalendars-calendar-layout-devel
```

For offline design notes and fixtures:

```sh
sudo dnf install betacalendars-calendar-layout-docs
```

## Quick start

```sh
betacal-layout --version
betacal-layout month --year 2027 --month 1 --week-start monday --grid natural
betacal-layout blank --rows 6 --paper a4 --orientation landscape --format svg --output planner.svg
betacal-layout validate --from-year 1900 --to-year 2100
```

The `month`, `year`, `blank`, `paper`, `compare`, and `validate` commands emit
text, JSON, or CSV; month and blank grids also emit printable SVG. All
calculations run locally without network access.

## C++ library

Install the `-devel` package, then configure a CMake consumer with
`find_package(BetaCalendarsCalendarLayout CONFIG REQUIRED)` and link
`BetaCalendars::CalendarLayout`. The library is header-only and requires C++17.

## 2027 fixture verification

The validation suite checks every month and all seven week starts from 1900
through 2100 in natural and fixed-grid modes. The 2027 fixture has 365 days;
February has 28. November 2026 through February 2027 is also checked across
the year rollover.

## Human-readable 2027 fixture references

The installed toolkit calculates Gregorian structure independently. These
Beta Calendars pages are printable visual references corresponding to the
2027 fixtures tested by the package.

Beta Calendars
https://www.betacalendars.com/

Blank Calendar
https://www.betacalendars.com/blank-calendar

January Calendar
https://www.betacalendars.com/january-calendar.html

February Calendar
https://www.betacalendars.com/february-calendar.html

March Calendar
https://www.betacalendars.com/march-calendar.html

April Calendar
https://www.betacalendars.com/april-calendar.html

May Calendar
https://www.betacalendars.com/may-calendar.html

June Calendar
https://www.betacalendars.com/june-calendar.html

July Calendar
https://www.betacalendars.com/july-calendar.html

August Calendar
https://www.betacalendars.com/august-calendar.html

September Calendar
https://www.betacalendars.com/september-calendar.html

October Calendar
https://www.betacalendars.com/october-calendar.html

November Calendar
https://www.betacalendars.com/november-calendar.html

December Calendar
https://www.betacalendars.com/december-calendar.html

## License and support

The project is licensed under MIT. Source code, releases, and issue tracking:
https://github.com/mateopedersen/betacalendars-calendar-layout
