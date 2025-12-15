#include "project_name/utils/string_utils.hpp"

#include <benchmark/benchmark.h>

static void BM_ToUpper(benchmark::State &state)
{
  for (auto _ : state)
  {
    benchmark::DoNotOptimize(project_name::utils::to_upper("cpp-quick-starter"));
  }
}

BENCHMARK(BM_ToUpper);

BENCHMARK_MAIN();
