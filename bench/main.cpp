#include <benchmark/benchmark.h>
#include <random>
#include <set>
#include <unordered_set>

#include "sparse-set.hpp"

// Random number generator setup
static std::mt19937 rng(42);

// Generate random integers
static std::vector<int> generate_random_ints(size_t count, int min = 0, int max = 1000000) {
    std::vector<int> result;
    result.reserve(count);
    std::uniform_int_distribution<int> dist(min, max);
    for (size_t i = 0; i < count; ++i) {
        result.push_back(dist(rng));
    }
    return result;
}

// ============================================================================
// INSERTION BENCHMARKS
// ============================================================================

static void BM_SparseSet_Insert(benchmark::State &state) {
    auto data = generate_random_ints(state.range(0));

    for (auto _ : state) {
        sparse_set<int> s;
        for (int val : data) {
            benchmark::DoNotOptimize(s.insert(val));
        }
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}

static void BM_StdSet_Insert(benchmark::State &state) {
    auto data = generate_random_ints(state.range(0));

    for (auto _ : state) {
        std::set<int> s;
        for (int val : data) {
            benchmark::DoNotOptimize(s.insert(val));
        }
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}

static void BM_UnorderedSet_Insert(benchmark::State &state) {
    auto data = generate_random_ints(state.range(0));

    for (auto _ : state) {
        std::unordered_set<int> s;
        for (int val : data) {
            benchmark::DoNotOptimize(s.insert(val));
        }
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_SparseSet_Insert)->Range(64, 1 << 16)->Complexity();
BENCHMARK(BM_StdSet_Insert)->Range(64, 1 << 16)->Complexity();
BENCHMARK(BM_UnorderedSet_Insert)->Range(64, 1 << 16)->Complexity();

// ============================================================================
// LOOKUP BENCHMARKS (with data present)
// ============================================================================

static void BM_SparseSet_Find_Hit(benchmark::State &state) {
    auto            data = generate_random_ints(state.range(0));
    sparse_set<int> s;
    for (int val : data) {
        s.insert(val);
    }

    for (auto _ : state) {
        for (int val : data) {
            benchmark::DoNotOptimize(s.find(val));
        }
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}

static void BM_StdSet_Find_Hit(benchmark::State &state) {
    auto          data = generate_random_ints(state.range(0));
    std::set<int> s;
    for (int val : data) {
        s.insert(val);
    }

    for (auto _ : state) {
        for (int val : data) {
            benchmark::DoNotOptimize(s.find(val));
        }
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}

static void BM_UnorderedSet_Find_Hit(benchmark::State &state) {
    auto                    data = generate_random_ints(state.range(0));
    std::unordered_set<int> s;
    for (int val : data) {
        s.insert(val);
    }

    for (auto _ : state) {
        for (int val : data) {
            benchmark::DoNotOptimize(s.find(val));
        }
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_SparseSet_Find_Hit)->Range(64, 1 << 16)->Complexity();
BENCHMARK(BM_StdSet_Find_Hit)->Range(64, 1 << 16)->Complexity();
BENCHMARK(BM_UnorderedSet_Find_Hit)->Range(64, 1 << 16)->Complexity();

// ============================================================================
// LOOKUP BENCHMARKS (with data NOT present)
// ============================================================================

static void BM_SparseSet_Find_Miss(benchmark::State &state) {
    auto            data      = generate_random_ints(state.range(0));
    auto            miss_data = generate_random_ints(state.range(0), 2000000, 3000000);
    sparse_set<int> s;
    for (int val : data) {
        s.insert(val);
    }

    for (auto _ : state) {
        for (int val : miss_data) {
            benchmark::DoNotOptimize(s.find(val));
        }
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}

static void BM_StdSet_Find_Miss(benchmark::State &state) {
    auto          data      = generate_random_ints(state.range(0));
    auto          miss_data = generate_random_ints(state.range(0), 2000000, 3000000);
    std::set<int> s;
    for (int val : data) {
        s.insert(val);
    }

    for (auto _ : state) {
        for (int val : miss_data) {
            benchmark::DoNotOptimize(s.find(val));
        }
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}

static void BM_UnorderedSet_Find_Miss(benchmark::State &state) {
    auto                    data      = generate_random_ints(state.range(0));
    auto                    miss_data = generate_random_ints(state.range(0), 2000000, 3000000);
    std::unordered_set<int> s;
    for (int val : data) {
        s.insert(val);
    }

    for (auto _ : state) {
        for (int val : miss_data) {
            benchmark::DoNotOptimize(s.find(val));
        }
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_SparseSet_Find_Miss)->Range(64, 1 << 16)->Complexity();
BENCHMARK(BM_StdSet_Find_Miss)->Range(64, 1 << 16)->Complexity();
BENCHMARK(BM_UnorderedSet_Find_Miss)->Range(64, 1 << 16)->Complexity();

// ============================================================================
// CONTAINS BENCHMARKS
// ============================================================================

static void BM_SparseSet_Contains(benchmark::State &state) {
    auto            data = generate_random_ints(state.range(0));
    sparse_set<int> s;
    for (int val : data) {
        s.insert(val);
    }

    for (auto _ : state) {
        for (int val : data) {
            benchmark::DoNotOptimize(s.contains(val));
        }
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}

static void BM_StdSet_Contains(benchmark::State &state) {
    auto          data = generate_random_ints(state.range(0));
    std::set<int> s;
    for (int val : data) {
        s.insert(val);
    }

    for (auto _ : state) {
        for (int val : data) {
            benchmark::DoNotOptimize(s.contains(val));
        }
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}

static void BM_UnorderedSet_Contains(benchmark::State &state) {
    auto                    data = generate_random_ints(state.range(0));
    std::unordered_set<int> s;
    for (int val : data) {
        s.insert(val);
    }

    for (auto _ : state) {
        for (int val : data) {
            benchmark::DoNotOptimize(s.contains(val));
        }
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_SparseSet_Contains)->Range(64, 1 << 16)->Complexity();
BENCHMARK(BM_StdSet_Contains)->Range(64, 1 << 16)->Complexity();
BENCHMARK(BM_UnorderedSet_Contains)->Range(64, 1 << 16)->Complexity();

// ============================================================================
// ERASE BENCHMARKS
// ============================================================================

static void BM_SparseSet_Erase(benchmark::State &state) {
    auto data = generate_random_ints(state.range(0));

    for (auto _ : state) {
        state.PauseTiming();
        sparse_set<int> s;
        for (int val : data) {
            s.insert(val);
        }
        state.ResumeTiming();

        for (int val : data) {
            benchmark::DoNotOptimize(s.erase(val));
        }
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}

static void BM_StdSet_Erase(benchmark::State &state) {
    auto data = generate_random_ints(state.range(0));

    for (auto _ : state) {
        state.PauseTiming();
        std::set<int> s;
        for (int val : data) {
            s.insert(val);
        }
        state.ResumeTiming();

        for (int val : data) {
            benchmark::DoNotOptimize(s.erase(val));
        }
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}

static void BM_UnorderedSet_Erase(benchmark::State &state) {
    auto data = generate_random_ints(state.range(0));

    for (auto _ : state) {
        state.PauseTiming();
        std::unordered_set<int> s;
        for (int val : data) {
            s.insert(val);
        }
        state.ResumeTiming();

        for (int val : data) {
            benchmark::DoNotOptimize(s.erase(val));
        }
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_SparseSet_Erase)->Range(64, 1 << 16)->Complexity();
BENCHMARK(BM_StdSet_Erase)->Range(64, 1 << 16)->Complexity();
BENCHMARK(BM_UnorderedSet_Erase)->Range(64, 1 << 16)->Complexity();

// ============================================================================
// ITERATION BENCHMARKS
// ============================================================================

static void BM_SparseSet_Iterate(benchmark::State &state) {
    auto            data = generate_random_ints(state.range(0));
    sparse_set<int> s;
    for (int val : data) {
        s.insert(val);
    }

    for (auto _ : state) {
        int sum = 0;
        for (int val : s) {
            sum += val;
        }
        benchmark::DoNotOptimize(sum);
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}

static void BM_StdSet_Iterate(benchmark::State &state) {
    auto          data = generate_random_ints(state.range(0));
    std::set<int> s;
    for (int val : data) {
        s.insert(val);
    }

    for (auto _ : state) {
        int sum = 0;
        for (int val : s) {
            sum += val;
        }
        benchmark::DoNotOptimize(sum);
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}

static void BM_UnorderedSet_Iterate(benchmark::State &state) {
    auto                    data = generate_random_ints(state.range(0));
    std::unordered_set<int> s;
    for (int val : data) {
        s.insert(val);
    }

    for (auto _ : state) {
        int sum = 0;
        for (int val : s) {
            sum += val;
        }
        benchmark::DoNotOptimize(sum);
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_SparseSet_Iterate)->Range(64, 1 << 16)->Complexity();
BENCHMARK(BM_StdSet_Iterate)->Range(64, 1 << 16)->Complexity();
BENCHMARK(BM_UnorderedSet_Iterate)->Range(64, 1 << 16)->Complexity();

// ============================================================================
// MIXED OPERATIONS BENCHMARK
// ============================================================================

static void BM_SparseSet_Mixed(benchmark::State &state) {
    auto data = generate_random_ints(state.range(0));

    for (auto _ : state) {
        sparse_set<int> s;

        // Insert phase
        for (size_t i = 0; i < data.size() / 2; ++i) {
            s.insert(data[i]);
        }

        // Mixed operations
        for (size_t i = 0; i < data.size() / 2; ++i) {
            benchmark::DoNotOptimize(s.contains(data[i]));
            s.insert(data[data.size() / 2 + i]);
            if (i % 3 == 0) {
                s.erase(data[i]);
            }
        }

        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}

static void BM_StdSet_Mixed(benchmark::State &state) {
    auto data = generate_random_ints(state.range(0));

    for (auto _ : state) {
        std::set<int> s;

        // Insert phase
        for (size_t i = 0; i < data.size() / 2; ++i) {
            s.insert(data[i]);
        }

        // Mixed operations
        for (size_t i = 0; i < data.size() / 2; ++i) {
            benchmark::DoNotOptimize(s.contains(data[i]));
            s.insert(data[data.size() / 2 + i]);
            if (i % 3 == 0) {
                s.erase(data[i]);
            }
        }

        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}

static void BM_UnorderedSet_Mixed(benchmark::State &state) {
    auto data = generate_random_ints(state.range(0));

    for (auto _ : state) {
        std::unordered_set<int> s;

        // Insert phase
        for (size_t i = 0; i < data.size() / 2; ++i) {
            s.insert(data[i]);
        }

        // Mixed operations
        for (size_t i = 0; i < data.size() / 2; ++i) {
            benchmark::DoNotOptimize(s.contains(data[i]));
            s.insert(data[data.size() / 2 + i]);
            if (i % 3 == 0) {
                s.erase(data[i]);
            }
        }

        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_SparseSet_Mixed)->Range(64, 1 << 16)->Complexity();
BENCHMARK(BM_StdSet_Mixed)->Range(64, 1 << 16)->Complexity();
BENCHMARK(BM_UnorderedSet_Mixed)->Range(64, 1 << 16)->Complexity();

BENCHMARK_MAIN();
