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
