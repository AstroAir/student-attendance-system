cmake_minimum_required(VERSION 3.21)

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

project(student_attendance_system VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(STUDENT_ATTENDANCE_ENABLE_WARNINGS "Enable compiler warnings" ON)
option(STUDENT_ATTENDANCE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(STUDENT_ATTENDANCE_ENABLE_SANITIZERS "Enable sanitizers" OFF)
option(STUDENT_ATTENDANCE_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
option(STUDENT_ATTENDANCE_ENABLE_CPPCHECK "Enable cppcheck" OFF)

option(STUDENT_ATTENDANCE_BUILD_SERVER "Build attendance server" ON)
option(STUDENT_ATTENDANCE_BUILD_TESTS "Build unit/integration tests" ON)
option(STUDENT_ATTENDANCE_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(STUDENT_ATTENDANCE_BUILD_EXAMPLES "Build examples" ON)
option(STUDENT_ATTENDANCE_BUILD_DOCS "Build docs (Doxygen)" OFF)

option(STUDENT_ATTENDANCE_ENABLE_MKDOCS "Enable MkDocs targets" ON)

set(_student_attendance_default_use_system_packages OFF)
if(MINGW)
  set(_student_attendance_default_use_system_packages ON)
endif()

option(STUDENT_ATTENDANCE_USE_SYSTEM_GTEST "Use system-installed GTest instead of FetchContent" ${_student_attendance_default_use_system_packages})
option(STUDENT_ATTENDANCE_USE_SYSTEM_BENCHMARK "Use system-installed benchmark instead of FetchContent" ${_student_attendance_default_use_system_packages})
option(STUDENT_ATTENDANCE_USE_SYSTEM_DROGON "Use system-installed Drogon instead of FetchContent" ${_student_attendance_default_use_system_packages})

# Main application (help/info only)
add_executable(student_attendance_app src/main.cpp)

if(STUDENT_ATTENDANCE_ENABLE_WARNINGS)
  include(CompilerWarnings)
  set_project_warnings(student_attendance_app ${STUDENT_ATTENDANCE_WARNINGS_AS_ERRORS})
endif()

if(STUDENT_ATTENDANCE_ENABLE_SANITIZERS)
  include(Sanitizers)
  enable_sanitizers(student_attendance_app)
endif()

include(StaticAnalyzers)
if(STUDENT_ATTENDANCE_ENABLE_CLANG_TIDY)
  enable_clang_tidy()
endif()
if(STUDENT_ATTENDANCE_ENABLE_CPPCHECK)
  enable_cppcheck()
endif()

include(FetchContent)

# Configure FetchContent cache directory for faster rebuilds
if(NOT FETCHCONTENT_BASE_DIR)
  set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/.cache/fetchcontent" CACHE PATH "FetchContent base dir")
endif()

# Enable FetchContent fully disconnected mode if sources already exist
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "" FORCE)

# ==================== Drogon Server ====================
if(STUDENT_ATTENDANCE_BUILD_SERVER)
  set(_student_attendance_use_system_drogon ${STUDENT_ATTENDANCE_USE_SYSTEM_DROGON})
  if(MSVC)
    set(_student_attendance_use_system_drogon OFF)
  endif()

  if(_student_attendance_use_system_drogon)
    find_package(Drogon CONFIG QUIET)
  endif()

  if(NOT Drogon_FOUND)
    # Check if drogon was already fetched
    if(EXISTS "${FETCHCONTENT_BASE_DIR}/drogon-src")
      message(STATUS "Using cached Drogon from ${FETCHCONTENT_BASE_DIR}/drogon-src")
    else()
      message(STATUS "Drogon not found, fetching from GitHub (will be cached for future builds)...")
    endif()

    FetchContent_Declare(drogon
      GIT_REPOSITORY https://github.com/drogonframework/drogon.git
      GIT_TAG v1.9.8
      GIT_SHALLOW TRUE
      GIT_PROGRESS TRUE
    )

    # Drogon build options
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_CTL ON CACHE BOOL "" FORCE)
    set(BUILD_ORM ON CACHE BOOL "" FORCE)
    set(BUILD_POSTGRESQL OFF CACHE BOOL "" FORCE)
    set(BUILD_MYSQL OFF CACHE BOOL "" FORCE)
    set(BUILD_SQLITE ON CACHE BOOL "" FORCE)
    set(BUILD_REDIS OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(drogon)
  endif()

  # Server library
  add_library(student_attendance_server_lib
    # Database
    src/db/DatabaseManager.cc
    # Legacy in-memory store (fallback)
    src/models/DataStore.cc
    # Services
    src/services/AuthService.cc
    src/services/StudentService.cc
    src/services/AttendanceService.cc
    src/services/ReportService.cc
    # Controllers
    src/controllers/AuthController.cc
    src/controllers/StudentController.cc
    src/controllers/AttendanceController.cc
    src/controllers/ReportController.cc
    src/controllers/DataController.cc
    src/controllers/ClassController.cc
    # Filters
    src/filters/AuthFilter.cc
  )
  add_library(student_attendance::server_lib ALIAS student_attendance_server_lib)

  target_include_directories(student_attendance_server_lib
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  )

  # Link Drogon - handle both FetchContent and find_package cases
  if(TARGET Drogon::Drogon)
    target_link_libraries(student_attendance_server_lib PUBLIC Drogon::Drogon)
  elseif(TARGET Drogon)
    target_link_libraries(student_attendance_server_lib PUBLIC Drogon)
  elseif(TARGET drogon)
    target_link_libraries(student_attendance_server_lib PUBLIC drogon)
  elseif(TARGET drogon::drogon)
    target_link_libraries(student_attendance_server_lib PUBLIC drogon::drogon)
  else()
    message(FATAL_ERROR "Drogon target not found")
  endif()

  target_compile_features(student_attendance_server_lib PUBLIC cxx_std_20)

  # Server executable
  add_executable(student_attendance_server src/server_main.cpp)
  target_link_libraries(student_attendance_server PRIVATE student_attendance::server_lib)

  # Copy config file
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.json
    ${CMAKE_CURRENT_BINARY_DIR}/config.json
    COPYONLY
  )
endif()

# ==================== Tests ====================
if(STUDENT_ATTENDANCE_BUILD_TESTS)
  include(CTest)
  enable_testing()

  set(_student_attendance_use_system_gtest ${STUDENT_ATTENDANCE_USE_SYSTEM_GTEST})
  if(MSVC)
    set(_student_attendance_use_system_gtest OFF)
  endif()

  if(_student_attendance_use_system_gtest)
    find_package(GTest CONFIG QUIET)
  endif()

  if(NOT GTest_FOUND)
    FetchContent_Declare(googletest
      URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
  endif()

  add_subdirectory(tests)
endif()

# ==================== Benchmarks ====================
if(STUDENT_ATTENDANCE_BUILD_BENCHMARKS)
  set(_student_attendance_use_system_benchmark ${STUDENT_ATTENDANCE_USE_SYSTEM_BENCHMARK})
  if(MSVC)
    set(_student_attendance_use_system_benchmark OFF)
  endif()

  if(_student_attendance_use_system_benchmark)
    find_package(benchmark CONFIG QUIET)
  endif()
  if(NOT benchmark_FOUND)
    FetchContent_Declare(benchmark
      URL https://github.com/google/benchmark/archive/refs/tags/v1.8.4.zip
    )
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(benchmark)
  endif()

  add_subdirectory(benchmarks)
endif()

# ==================== Documentation ====================
if(STUDENT_ATTENDANCE_BUILD_DOCS)
  include(Doxygen)
  add_doxygen_target()
endif()

if(STUDENT_ATTENDANCE_ENABLE_MKDOCS)
  include(MkDocs)
  add_mkdocs_targets(
    CONFIG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/mkdocs.yml"
    SITE_DIR "${CMAKE_BINARY_DIR}/mkdocs"
  )
endif()

add_custom_target(docs)
if(TARGET doxygen)
  add_dependencies(docs doxygen)
endif()
if(TARGET mkdocs-build)
  add_dependencies(docs mkdocs-build)
endif()
