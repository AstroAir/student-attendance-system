add_executable(benchmarks
  string_utils_bench.cpp
)

target_link_libraries(benchmarks
  PRIVATE
    cpp_quick_starter::cpp_quick_starter
)

set(_benchmark_target "")
if(TARGET benchmark::benchmark)
  set(_benchmark_target benchmark::benchmark)
elseif(TARGET benchmark)
  set(_benchmark_target benchmark)
endif()

if(NOT _benchmark_target)
  message(FATAL_ERROR "Google Benchmark target not found. Enable CPP_QUICK_STARTER_BUILD_BENCHMARKS only when benchmark is available.")
endif()

target_link_libraries(benchmarks PRIVATE ${_benchmark_target})
