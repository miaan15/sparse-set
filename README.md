# Project Overview

Single file, header-only implementation of sparse set data structure.

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

## Example
```cpp
#include <algorithm>
#include <iostream>
#include <string>

#include "sparse-set.hpp"

int main() {
    sparse_set<int> int_set;

    int_set.insert(10);
    int_set.emplace(20);
    int_set.insert(30);

    std::cout << "Elements in the set: ";
    for (const auto &element : int_set) {
        std::cout << element << " ";
    }
    std::cout << "\n";

    if (auto it = int_set.find(20); it != int_set.end()) {
        std::cout << "Found element: " << *it << "\n";
    }

    int_set.erase(20);

    std::cout << "Elements after erasing 20: ";
    std::ranges::for_each(int_set, [](const auto &element) -> auto {
        std::cout << element << " ";
    });
    std::cout << "\n";

    sparse_set<std::string> string_set;

    string_set.insert("hello");
    string_set.emplace("world");

    std::cout << "String elements: ";
    for (const auto &element : string_set) {
        std::cout << element << " ";
    }
    std::cout << "\n";

    return 0;
}
```
```
Elements in the set: 10 20 30 
Found element: 20
Elements after erasing 20: 10 30 
String elements: hello world 
```
