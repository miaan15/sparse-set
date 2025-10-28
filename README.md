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
    std::cout << "::int set" << "\n";

    int_set.insert(10);
    std::cout << "insert " << 10 << "\n";
    int_set.emplace(20);
    std::cout << "emplace " << 20 << "\n";
    int_set.insert(30);
    std::cout << "insert " << 30 << "\n";

    std::cout << "elements: ";
    for (const auto &element : int_set) {
        std::cout << element << " ";
    }
    std::cout << "\n";

    if (auto it = int_set.find(20); it != int_set.end()) {
        std::cout << "found: " << *it << "\n";
    }

    int_set.erase(20);
    std::cout << "erase " << 20 << "\n";

    std::cout << "elements: ";
    std::ranges::for_each(int_set, [](const auto &element) -> auto {
        std::cout << element << " ";
    });
    std::cout << "\n\n";

    sparse_set<std::string> string_set;
    std::cout << "::string set" << "\n";

    string_set.insert("hello");
    string_set.emplace("sparse");

    std::cout << "elements: ";
    for (const auto &element : string_set) {
        std::cout << element << " ";
    }
    std::cout << std::endl;

    return 0;
}
```
```
::int set
insert 10
emplace 20
insert 30
elements: 10 20 30 
found: 20
erase 20
elements: 10 30 

::string set
elements: hello sparse 
```
