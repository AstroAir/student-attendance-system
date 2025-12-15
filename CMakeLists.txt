cmake_minimum_required(VERSION 3.21)

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

project(cpp_quick_starter VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(CPP_QUICK_STARTER_ENABLE_WARNINGS "Enable compiler warnings" ON)
option(CPP_QUICK_STARTER_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(CPP_QUICK_STARTER_ENABLE_SANITIZERS "Enable sanitizers" OFF)
option(CPP_QUICK_STARTER_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
option(CPP_QUICK_STARTER_ENABLE_CPPCHECK "Enable cppcheck" OFF)

option(CPP_QUICK_STARTER_BUILD_TESTS "Build unit/integration tests" ON)
option(CPP_QUICK_STARTER_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(CPP_QUICK_STARTER_BUILD_EXAMPLES "Build examples" ON)
option(CPP_QUICK_STARTER_BUILD_DOCS "Build docs (Doxygen)" OFF)

option(CPP_QUICK_STARTER_ENABLE_MKDOCS "Enable MkDocs targets" ON)

set(_cpp_quick_starter_default_use_system_packages OFF)
if(MINGW)
  set(_cpp_quick_starter_default_use_system_packages ON)
endif()

option(CPP_QUICK_STARTER_USE_SYSTEM_GTEST "Use system-installed GTest instead of FetchContent" ${_cpp_quick_starter_default_use_system_packages})
option(CPP_QUICK_STARTER_USE_SYSTEM_BENCHMARK "Use system-installed benchmark instead of FetchContent" ${_cpp_quick_starter_default_use_system_packages})

add_library(cpp_quick_starter
  src/core/greeting.cpp
  src/utils/string_utils.cpp
)
add_library(cpp_quick_starter::cpp_quick_starter ALIAS cpp_quick_starter)

target_include_directories(cpp_quick_starter
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_features(cpp_quick_starter PUBLIC cxx_std_20)

if(CPP_QUICK_STARTER_ENABLE_WARNINGS)
  include(CompilerWarnings)
  set_project_warnings(cpp_quick_starter ${CPP_QUICK_STARTER_WARNINGS_AS_ERRORS})
endif()

if(CPP_QUICK_STARTER_ENABLE_SANITIZERS)
  include(Sanitizers)
  enable_sanitizers(cpp_quick_starter)
endif()

include(StaticAnalyzers)
if(CPP_QUICK_STARTER_ENABLE_CLANG_TIDY)
  enable_clang_tidy()
endif()
if(CPP_QUICK_STARTER_ENABLE_CPPCHECK)
  enable_cppcheck()
endif()

add_executable(cpp_quick_starter_app src/main.cpp)
target_link_libraries(cpp_quick_starter_app PRIVATE cpp_quick_starter::cpp_quick_starter)

if(CPP_QUICK_STARTER_BUILD_EXAMPLES)
  add_executable(example_01 examples/example_01.cpp)
  target_link_libraries(example_01 PRIVATE cpp_quick_starter::cpp_quick_starter)
endif()

include(FetchContent)

if(CPP_QUICK_STARTER_BUILD_TESTS)
  include(CTest)
  enable_testing()

  set(_cpp_quick_starter_use_system_gtest ${CPP_QUICK_STARTER_USE_SYSTEM_GTEST})
  if(MSVC)
    set(_cpp_quick_starter_use_system_gtest OFF)
  endif()

  if(_cpp_quick_starter_use_system_gtest)
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

if(CPP_QUICK_STARTER_BUILD_BENCHMARKS)
  set(_cpp_quick_starter_use_system_benchmark ${CPP_QUICK_STARTER_USE_SYSTEM_BENCHMARK})
  if(MSVC)
    set(_cpp_quick_starter_use_system_benchmark OFF)
  endif()

  if(_cpp_quick_starter_use_system_benchmark)
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

if(CPP_QUICK_STARTER_BUILD_DOCS)
  include(Doxygen)
  add_doxygen_target()
endif()

if(CPP_QUICK_STARTER_ENABLE_MKDOCS)
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
