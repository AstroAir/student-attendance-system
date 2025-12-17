# Find GTest target
set(_gtest_target "")
if(TARGET GTest::gtest)
  set(_gtest_target GTest::gtest)
elseif(TARGET gtest)
  set(_gtest_target gtest)
endif()

if(NOT _gtest_target)
  message(FATAL_ERROR "GoogleTest target not found. Ensure GTest is available (FetchContent/vcpkg/Conan).")
endif()

include(GoogleTest)

# ==================== API Tests ====================
if(STUDENT_ATTENDANCE_BUILD_SERVER AND TARGET student_attendance::server_lib)
  add_executable(api_tests
    api/main.cpp
    api/student_api_test.cpp
    api/attendance_api_test.cpp
    api/report_api_test.cpp
    api/data_api_test.cpp
    api/class_api_test.cpp
    api/auth_api_test.cpp
    api/models_test.cpp
    api/utils_test.cpp
    api/database_test.cpp
  )

  target_link_libraries(api_tests
    PRIVATE
      student_attendance::server_lib
      ${_gtest_target}
  )

  gtest_discover_tests(api_tests
    DISCOVERY_MODE PRE_TEST
  )
endif()
