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

static void BM_SparseSetContains(benchmark::State& state) {
  sparse_set<int> set;
  for (int i = 0; i < state.range(0); ++i) {
    set.insert(i);
  }
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(set.contains(i));
    }
  }
}
BENCHMARK(BM_SparseSetContains)->Range(8, 8<<10);

static void BM_SparseSetErase(benchmark::State& state) {
  sparse_set<int> set;
  for (auto _ : state) {
    state.PauseTiming();
    for (int i = 0; i < state.range(0); ++i) {
      set.insert(i);
    }
    state.ResumeTiming();
    for (int i = 0; i < state.range(0); ++i) {
      set.erase(i);
    }
  }
}
BENCHMARK(BM_SparseSetErase)->Range(8, 8<<10);

BENCHMARK_MAIN();