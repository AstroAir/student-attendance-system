add_executable(unit_tests
  unit/main.cpp
  unit/greeting_test.cpp
  unit/string_utils_test.cpp
)

target_link_libraries(unit_tests
  PRIVATE
    cpp_quick_starter::cpp_quick_starter
)

set(_gtest_target "")
if(TARGET GTest::gtest)
  set(_gtest_target GTest::gtest)
elseif(TARGET gtest)
  set(_gtest_target gtest)
endif()

if(NOT _gtest_target)
  message(FATAL_ERROR "GoogleTest target not found. Ensure GTest is available (FetchContent/vcpkg/Conan).")
endif()

target_link_libraries(unit_tests PRIVATE ${_gtest_target})

include(GoogleTest)

gtest_discover_tests(unit_tests
  DISCOVERY_MODE PRE_TEST
)

add_executable(integration_tests
  integration/main.cpp
  integration/smoke_test.cpp
)

target_link_libraries(integration_tests
  PRIVATE
    cpp_quick_starter::cpp_quick_starter
)

target_link_libraries(integration_tests PRIVATE ${_gtest_target})

gtest_discover_tests(integration_tests
  DISCOVERY_MODE PRE_TEST
)
