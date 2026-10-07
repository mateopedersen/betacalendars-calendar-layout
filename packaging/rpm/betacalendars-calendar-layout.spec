Name:           betacalendars-calendar-layout
Version:        0.1.0
Release:        1%{?dist}
Summary:        Deterministic Gregorian calendar grids and print geometry
License:        MIT
URL:            https://www.betacalendars.com/
Source0:        https://github.com/mateopedersen/betacalendars-calendar-layout/archive/refs/tags/v%{version}.tar.gz

BuildRequires:  cmake >= 3.20
BuildRequires:  gcc-c++
BuildRequires:  ninja-build

%description
BetaCalendars Calendar Layout is a dependency-light C++17 library and offline
command-line toolkit for deterministic Gregorian month grids, seven week-start
conventions, undated planner layouts, and physical printable-page geometry.
It supports A4, A5, US Letter, and US Legal, and emits text, JSON, CSV, and SVG.

%package devel
Summary:        Headers and CMake package files for BetaCalendars Calendar Layout

%description devel
Development headers and the exported CMake package for applications using the
BetaCalendars Calendar Layout interface library.

%package docs
Summary:        Offline documentation for BetaCalendars Calendar Layout

%description docs
Design, calendar-topology, page-geometry, blank-grid, 2027 fixture, and
year-boundary documentation for BetaCalendars Calendar Layout.

%prep
%autosetup -n %{name}-%{version}

%build
%cmake -DCMAKE_BUILD_TYPE=Release \
  -DBETACALENDARS_BUILD_TESTS=ON \
  -DBETACALENDARS_BUILD_TOOLS=ON
%cmake_build

%check
ctest --test-dir build --output-on-failure

# Verify the installed CMake package can be consumed downstream.
cmake --install build --prefix %{_builddir}/betacalendars-consumer-stage
cmake -S tests/consumer -B consumer-build -G Ninja \
  -DCMAKE_PREFIX_PATH=%{_builddir}/betacalendars-consumer-stage \
  -DCMAKE_BUILD_TYPE=Release
cmake --build consumer-build --parallel
./consumer-build/calendar-layout-consumer

%install
%cmake_install

%files
%license LICENSE
%{_bindir}/betacal-layout

%files devel
%license LICENSE
%{_includedir}/betacalendars/
%{_libdir}/cmake/BetaCalendarsCalendarLayout/

%files docs
%license LICENSE
%doc README.md CHANGELOG.md CONTRIBUTING.md docs/ tests/fixtures/

%changelog
* Wed Oct 07 2026 Mateo Pedersen <mateopedersen@users.noreply.github.com> - 0.1.0-1
- Initial Fedora Copr package.
