#include <benchmark/benchmark.h>
#include "sparse-set.hpp"

static void BM_SparseSetInsert(benchmark::State& state) {
  sparse_set<int> set;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      set.insert(i);
    }
  }
}
BENCHMARK(BM_SparseSetInsert)->Range(8, 8<<10);

BENCHMARK_MAIN();
