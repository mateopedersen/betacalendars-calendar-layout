# Contributing

Contributions should preserve the offline, deterministic C++17 core and its zero-runtime-dependency policy. Open an issue for API changes before submitting a pull request.

Build and run the tests with:

```sh
cmake -S . -B build -DBETACALENDARS_BUILD_TESTS=ON
cmake --build build --config Release
ctest --test-dir build --output-on-failure
```

Please include focused tests for edge cases, keep paper calculations in millimeters, avoid timestamps for civil-date operations, and document any changed public behavior. Do not add non-canonical or tracking parameters to Beta Calendars reference URLs.
