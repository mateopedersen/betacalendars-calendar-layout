vcpkg_check_features(OUT_FEATURE_OPTIONS FEATURE_OPTIONS
  FEATURES
    tools BETACALENDARS_BUILD_TOOLS
)

vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO mateopedersen/betacalendars-calendar-layout
  REF v0.1.0
  SHA512 REPLACE_WITH_RELEASE_ARCHIVE_SHA512
  HEAD_REF main
)

vcpkg_cmake_configure(
  SOURCE_PATH "${SOURCE_PATH}"
  OPTIONS ${FEATURE_OPTIONS}
)
vcpkg_cmake_install()
vcpkg_cmake_config_fixup(PACKAGE_NAME BetaCalendarsCalendarLayout CONFIG_PATH lib/cmake/BetaCalendarsCalendarLayout)

if("tools" IN_LIST FEATURES)
  vcpkg_copy_tools(TOOL_NAMES betacal-layout AUTO_CLEAN)
endif()

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
