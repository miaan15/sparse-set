#include <cassert>
#include <iostream>
#include <print>
#include <unordered_set>

#include "sparse-set.hpp"

int main() {
    sparse_set<int> int_set;

    // Insert some elements
    std::cout << "Inserting 10, 20, 30" << "\n";
    int_set.insert(10);
    int_set.insert(20);
    int_set.insert(30);
    int_set.insert(73);

    // Print the size of the set
    std::cout << "Size of the set: " << int_set.size() << "\n";

    // Check if the set contains an element
    std::cout << "Contains 20? " << (int_set.contains(20) ? "Yes" : "No") << "\n";
    std::cout << "Contains 40? " << (int_set.contains(40) ? "Yes" : "No") << "\n";

    // Erase an element
    std::cout << "Erasing 20" << "\n";
    int_set.erase(20);

    // Print the size of the set again
    std::cout << "Size of the set: " << int_set.size() << "\n";

    // Check if the set still contains the erased element
    std::cout << "Contains 20? " << (int_set.contains(20) ? "Yes" : "No") << "\n";
    //
    // // Insert enough elements to trigger rehashing
    // // Initial capacity is 64, load factor is 0.75
    // // So should rehash at 48 elements (floor(64 * 0.75))
    // for (int i = 0; i < 100; ++i) {
    //     int_set.insert(i);
    // }
    //
    // std::cout << "Size of the set: " << int_set.size() << "\n";
    //
    // assert(int_set.size() == 100);
    //
    // // Verify all elements are still accessible
    // for (int i = 0; i < 100; ++i) {
    //     assert(int_set.contains(i));
    // }

    return 0;
}
