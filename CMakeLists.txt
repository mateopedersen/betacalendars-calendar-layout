cmake_minimum_required(VERSION 3.20)
project(BetaCalendarsCalendarLayout VERSION 0.1.1 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(betacalendars-calendar-layout INTERFACE)
add_library(BetaCalendars::CalendarLayout ALIAS betacalendars-calendar-layout)
set_target_properties(betacalendars-calendar-layout PROPERTIES EXPORT_NAME CalendarLayout)
target_compile_features(betacalendars-calendar-layout INTERFACE cxx_std_17)
target_include_directories(betacalendars-calendar-layout INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

option(BETACALENDARS_BUILD_TESTS "Build the self-contained test suite" OFF)
option(BETACALENDARS_BUILD_TOOLS "Build the optional command-line tool" OFF)
if(BETACALENDARS_BUILD_TESTS)
  enable_testing()
  add_executable(calendar-layout-tests tests/test_calendar_layout.cpp)
  target_link_libraries(calendar-layout-tests PRIVATE BetaCalendars::CalendarLayout)
  add_test(NAME calendar-layout-tests COMMAND calendar-layout-tests)
endif()
if(BETACALENDARS_BUILD_TOOLS)
  add_executable(betacal-layout tools/betacal-layout.cpp)
  target_link_libraries(betacal-layout PRIVATE BetaCalendars::CalendarLayout)
  install(TARGETS betacal-layout RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
  if(BETACALENDARS_BUILD_TESTS)
    add_test(NAME cli-version COMMAND betacal-layout --version)
    add_test(NAME cli-january-2027 COMMAND betacal-layout month --year 2027 --month 1 --week-start monday --grid natural --format json)
    add_test(NAME cli-blank-a4 COMMAND betacal-layout blank --rows 6 --paper a4 --orientation landscape --format svg)
    add_test(NAME cli-exhaustive-validation COMMAND betacal-layout validate --from-year 1900 --to-year 2100)
  endif()
endif()

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS betacalendars-calendar-layout EXPORT BetaCalendarsCalendarLayoutTargets)
install(EXPORT BetaCalendarsCalendarLayoutTargets
  FILE BetaCalendarsCalendarLayoutTargets.cmake
  NAMESPACE BetaCalendars:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BetaCalendarsCalendarLayout)
configure_package_config_file(cmake/BetaCalendarsCalendarLayoutConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/BetaCalendarsCalendarLayoutConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BetaCalendarsCalendarLayout)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/BetaCalendarsCalendarLayoutConfigVersion.cmake
  VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/BetaCalendarsCalendarLayoutConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/BetaCalendarsCalendarLayoutConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BetaCalendarsCalendarLayout)
