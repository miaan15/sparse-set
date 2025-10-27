# Project Overview

This project is a header-only C++ library that implements a sparse set data structure.
Pretty much have the same API as std::set (maybe)

## Build & Run

```bash
cmake -S . -B build -DBUILD_TEST=ON -DBUILD_BENCH=ON -DBUILD_DEMO=ON
cmake --build build
```

## Tests

```bash
ctest --test-dir build/test --output-on-failure
```

## Benchmarks

```bash
./build/bench/sparse_set_bench
```
