// AI-generated test
// (wtf it is so long)

#include <algorithm>
#include <gtest/gtest.h>
#include <numeric>
#include <string>
#include <unordered_set>

#include "sparse-set.hpp"

#ifdef __GNUC__
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wsign-compare"
#    pragma GCC diagnostic ignored "-Wsign-conversion"
#endif

#ifdef __clang__
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wsign-compare"
#    pragma clang diagnostic ignored "-Wsign-conversion"
#    pragma clang diagnostic ignored "-Wshorten-64-to-32"
#endif

#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 4018)
#    pragma warning(disable : 4244)
#    pragma warning(disable : 4267)
#endif

// ============================================================================
// Test Fixture
// ============================================================================

class SparseSetTest : public ::testing::Test {
protected:
    sparse_set<int>         int_set;
    sparse_set<std::string> str_set;
};

// ============================================================================
// Constructor and Assignment Tests
// ============================================================================

TEST_F(SparseSetTest, DefaultConstructor) {
    sparse_set<int> set;
    EXPECT_EQ(set.size(), 0);
    EXPECT_TRUE(set.empty());
}

TEST_F(SparseSetTest, CopyConstructor) {
    int_set.insert(1);
    int_set.insert(2);
    int_set.insert(3);

    sparse_set<int> copied(int_set);
    EXPECT_EQ(copied.size(), 3);
    EXPECT_TRUE(copied.contains(1));
    EXPECT_TRUE(copied.contains(2));
    EXPECT_TRUE(copied.contains(3));
}

TEST_F(SparseSetTest, MoveConstructor) {
    int_set.insert(1);
    int_set.insert(2);
    int_set.insert(3);

    sparse_set<int> moved(std::move(int_set));
    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(moved.contains(1));
    EXPECT_TRUE(moved.contains(2));
    EXPECT_TRUE(moved.contains(3));
}

TEST_F(SparseSetTest, CopyAssignment) {
    int_set.insert(1);
    int_set.insert(2);

    sparse_set<int> other;
    other.insert(99);
    other = int_set;

    EXPECT_EQ(other.size(), 2);
    EXPECT_TRUE(other.contains(1));
    EXPECT_TRUE(other.contains(2));
    EXPECT_FALSE(other.contains(99));
}

TEST_F(SparseSetTest, MoveAssignment) {
    int_set.insert(1);
    int_set.insert(2);

    sparse_set<int> other;
    other.insert(99);
    other = std::move(int_set);

    EXPECT_EQ(other.size(), 2);
    EXPECT_TRUE(other.contains(1));
    EXPECT_TRUE(other.contains(2));
}

// ============================================================================
// Insert Tests
// ============================================================================

TEST_F(SparseSetTest, InsertSingleElement) {
    auto [it, inserted] = int_set.insert(42);
    EXPECT_TRUE(inserted);
    EXPECT_EQ(*it, 42);
    EXPECT_EQ(int_set.size(), 1);
    EXPECT_TRUE(int_set.contains(42));
}

TEST_F(SparseSetTest, InsertMultipleElements) {
    auto [it1, ins1] = int_set.insert(1);
    auto [it2, ins2] = int_set.insert(2);
    auto [it3, ins3] = int_set.insert(3);

    EXPECT_TRUE(ins1 && ins2 && ins3);
    EXPECT_EQ(int_set.size(), 3);
    EXPECT_TRUE(int_set.contains(1));
    EXPECT_TRUE(int_set.contains(2));
    EXPECT_TRUE(int_set.contains(3));
}

TEST_F(SparseSetTest, InsertDuplicate) {
    auto [it1, ins1] = int_set.insert(42);
    auto [it2, ins2] = int_set.insert(42);

    EXPECT_TRUE(ins1);
    EXPECT_FALSE(ins2);
    EXPECT_EQ(int_set.size(), 1);
}

TEST_F(SparseSetTest, InsertRvalue) {
    auto [it, inserted] = int_set.insert(std::move(42));
    EXPECT_TRUE(inserted);
    EXPECT_EQ(*it, 42);
    EXPECT_EQ(int_set.size(), 1);
}

TEST_F(SparseSetTest, InsertStrings) {
    auto [it1, ins1] = str_set.insert("hello");
    auto [it2, ins2] = str_set.insert("world");
    auto [it3, ins3] = str_set.insert("hello");

    EXPECT_TRUE(ins1 && ins2);
    EXPECT_FALSE(ins3);
    EXPECT_EQ(str_set.size(), 2);
    EXPECT_TRUE(str_set.contains("hello"));
    EXPECT_TRUE(str_set.contains("world"));
}

TEST_F(SparseSetTest, InsertZero) {
    auto [it, inserted] = int_set.insert(0);
    EXPECT_TRUE(inserted);
    EXPECT_TRUE(int_set.contains(0));
    EXPECT_EQ(int_set.size(), 1);
}

TEST_F(SparseSetTest, InsertNegativeNumbers) {
    int_set.insert(-1);
    int_set.insert(-100);
    int_set.insert(-42);

    EXPECT_TRUE(int_set.contains(-1));
    EXPECT_TRUE(int_set.contains(-100));
    EXPECT_TRUE(int_set.contains(-42));
    EXPECT_EQ(int_set.size(), 3);
}

TEST_F(SparseSetTest, InsertArrayOfNumbers) {
    std::vector<int> a = {1, 2, 3};

    int_set.insert(a.begin(), a.end());

    EXPECT_TRUE(int_set.contains(1));
    EXPECT_TRUE(int_set.contains(2));
    EXPECT_TRUE(int_set.contains(3));
    EXPECT_EQ(int_set.size(), 3);

    int_set.insert({1, 3, 5});

    EXPECT_TRUE(int_set.contains(1));
    EXPECT_TRUE(int_set.contains(2));
    EXPECT_TRUE(int_set.contains(3));
    EXPECT_TRUE(int_set.contains(5));
    EXPECT_EQ(int_set.size(), 4);
}

TEST_F(SparseSetTest, InsertRanges) {
    int_set.insert_range(std::views::iota(1, 4));

    EXPECT_TRUE(int_set.contains(1));
    EXPECT_TRUE(int_set.contains(2));
    EXPECT_TRUE(int_set.contains(3));
    EXPECT_EQ(int_set.size(), 3);

    int_set.insert_range(std::views::iota(1, 6) | std::views::filter([](int x) -> bool {
                             return x % 2 == 1;
                         }));

    EXPECT_TRUE(int_set.contains(5));
    EXPECT_EQ(int_set.size(), 4);

    int_set.insert_range(
        std::views::iota(4, 12) | std::views::filter([](int x) -> bool { return x % 2 == 0; })
        | std::views::transform([](int x) -> int { return x; })
    );

    EXPECT_TRUE(int_set.contains(4));
    EXPECT_TRUE(int_set.contains(6));
    EXPECT_TRUE(int_set.contains(8));
    EXPECT_TRUE(int_set.contains(10));
    EXPECT_EQ(int_set.size(), 8);

    std::vector<int> extra{3, 7, 7, 9};
    int_set.insert_range(extra | std::views::all);

    EXPECT_TRUE(int_set.contains(7));
    EXPECT_TRUE(int_set.contains(9));
    EXPECT_EQ(int_set.size(), 10);
}

// ============================================================================
// Emplace Tests
// ============================================================================

TEST_F(SparseSetTest, EmplaceElement) {
    auto [it, inserted] = int_set.emplace(42);
    EXPECT_TRUE(inserted);
    EXPECT_EQ(*it, 42);
    EXPECT_EQ(int_set.size(), 1);
}

TEST_F(SparseSetTest, EmplaceString) {
    auto [it, inserted] = str_set.emplace("test");
    EXPECT_TRUE(inserted);
    EXPECT_EQ(*it, "test");
    EXPECT_TRUE(str_set.contains("test"));
}

TEST_F(SparseSetTest, EmplaceDuplicate) {
    int_set.emplace(10);
    auto [it, inserted] = int_set.emplace(10);
    EXPECT_FALSE(inserted);
    EXPECT_EQ(int_set.size(), 1);
}

// ============================================================================
// Erase Tests
// ============================================================================

TEST_F(SparseSetTest, EraseExistingElement) {
    int_set.insert(42);
    EXPECT_EQ(int_set.erase(42), 1);
    EXPECT_EQ(int_set.size(), 0);
    EXPECT_FALSE(int_set.contains(42));
}

TEST_F(SparseSetTest, EraseNonExistingElement) {
    int_set.insert(42);
    EXPECT_EQ(int_set.erase(99), 0);
    EXPECT_EQ(int_set.size(), 1);
}

TEST_F(SparseSetTest, EraseFromEmptySet) {
    EXPECT_EQ(int_set.erase(42), 0);
    EXPECT_EQ(int_set.size(), 0);
}

TEST_F(SparseSetTest, EraseMultipleElements) {
    int_set.insert(1);
    int_set.insert(2);
    int_set.insert(3);

    EXPECT_EQ(int_set.erase(2), 1);
    EXPECT_EQ(int_set.size(), 2);
    EXPECT_TRUE(int_set.contains(1));
    EXPECT_FALSE(int_set.contains(2));
    EXPECT_TRUE(int_set.contains(3));

    EXPECT_EQ(int_set.erase(1), 1);
    EXPECT_EQ(int_set.size(), 1);
    EXPECT_FALSE(int_set.contains(1));
    EXPECT_TRUE(int_set.contains(3));

    EXPECT_EQ(int_set.erase(3), 1);
    EXPECT_EQ(int_set.size(), 0);
    EXPECT_FALSE(int_set.contains(3));
}

TEST_F(SparseSetTest, EraseAndReinsert) {
    int_set.insert(42);
    int_set.erase(42);
    auto [it, inserted] = int_set.insert(42);
    EXPECT_TRUE(inserted);
    EXPECT_TRUE(int_set.contains(42));
    EXPECT_EQ(int_set.size(), 1);
}

TEST_F(SparseSetTest, EraseLastElement) {
    int_set.insert(1);
    int_set.insert(2);
    int_set.insert(3);

    EXPECT_EQ(int_set.erase(3), 1);
    EXPECT_FALSE(int_set.contains(3));
    EXPECT_TRUE(int_set.contains(1));
    EXPECT_TRUE(int_set.contains(2));
}

TEST_F(SparseSetTest, EraseFirstElement) {
    int_set.insert(1);
    int_set.insert(2);
    int_set.insert(3);

    EXPECT_EQ(int_set.erase(1), 1);
    EXPECT_FALSE(int_set.contains(1));
    EXPECT_TRUE(int_set.contains(2));
    EXPECT_TRUE(int_set.contains(3));
}

TEST_F(SparseSetTest, EraseMiddleElement) {
    int_set.insert(1);
    int_set.insert(2);
    int_set.insert(3);

    EXPECT_EQ(int_set.erase(2), 1);
    EXPECT_TRUE(int_set.contains(1));
    EXPECT_FALSE(int_set.contains(2));
    EXPECT_TRUE(int_set.contains(3));
}

// ============================================================================
// Contains and Count Tests
// ============================================================================

TEST_F(SparseSetTest, ContainsExistingElement) {
    int_set.insert(42);
    EXPECT_TRUE(int_set.contains(42));
}

TEST_F(SparseSetTest, ContainsNonExistingElement) {
    int_set.insert(42);
    EXPECT_FALSE(int_set.contains(99));
}

TEST_F(SparseSetTest, ContainsInEmptySet) {
    EXPECT_FALSE(int_set.contains(42));
}

TEST_F(SparseSetTest, ContainsAfterErase) {
    int_set.insert(42);
    int_set.erase(42);
    EXPECT_FALSE(int_set.contains(42));
}

TEST_F(SparseSetTest, CountExistingElement) {
    int_set.insert(42);
    EXPECT_EQ(int_set.count(42), 1);
}

TEST_F(SparseSetTest, CountNonExistingElement) {
    int_set.insert(42);
    EXPECT_EQ(int_set.count(99), 0);
}

TEST_F(SparseSetTest, CountInEmptySet) {
    EXPECT_EQ(int_set.count(42), 0);
}

// ============================================================================
// Find Tests
// ============================================================================

TEST_F(SparseSetTest, FindExistingElement) {
    int_set.insert(10);
    int_set.insert(20);
    int_set.insert(30);

    auto it = int_set.find(20);
    EXPECT_NE(it, int_set.end());
    EXPECT_EQ(*it, 20);
}

TEST_F(SparseSetTest, FindNonExistingElement) {
    int_set.insert(10);
    int_set.insert(20);

    auto it = int_set.find(99);
    EXPECT_EQ(it, int_set.end());
}

TEST_F(SparseSetTest, FindInEmptySet) {
    auto it = int_set.find(42);
    EXPECT_EQ(it, int_set.end());
}

TEST_F(SparseSetTest, FindConstVersion) {
    int_set.insert(10);
    int_set.insert(20);
    int_set.insert(30);

    const auto &const_set = int_set;
    auto        it        = const_set.find(20);
    EXPECT_NE(it, const_set.end());
    EXPECT_EQ(*it, 20);
}

TEST_F(SparseSetTest, FindAfterErase) {
    int_set.insert(10);
    int_set.insert(20);
    int_set.insert(30);

    int_set.erase(20);

    auto it = int_set.find(20);
    EXPECT_EQ(it, int_set.end());

    auto it2 = int_set.find(10);
    EXPECT_NE(it2, int_set.end());
    EXPECT_EQ(*it2, 10);
}

TEST_F(SparseSetTest, FindAllElements) {
    std::vector<int> values = {1, 2, 3, 4, 5};
    for (int val : values) {
        int_set.insert(val);
    }

    for (int val : values) {
        auto it = int_set.find(val);
        EXPECT_NE(it, int_set.end());
        EXPECT_EQ(*it, val);
    }
}

TEST_F(SparseSetTest, FindWithStrings) {
    str_set.insert("hello");
    str_set.insert("world");
    str_set.insert("test");

    auto it1 = str_set.find("world");
    EXPECT_NE(it1, str_set.end());
    EXPECT_EQ(*it1, "world");

    auto it2 = str_set.find("nonexistent");
    EXPECT_EQ(it2, str_set.end());
}

// ============================================================================
// Size and Empty Tests
// ============================================================================

TEST_F(SparseSetTest, SizeEmpty) {
    EXPECT_EQ(int_set.size(), 0);
    EXPECT_TRUE(int_set.empty());
}

TEST_F(SparseSetTest, SizeAfterInserts) {
    int_set.insert(1);
    EXPECT_EQ(int_set.size(), 1);
    EXPECT_FALSE(int_set.empty());

    int_set.insert(2);
    EXPECT_EQ(int_set.size(), 2);

    int_set.insert(3);
    EXPECT_EQ(int_set.size(), 3);
}

TEST_F(SparseSetTest, SizeAfterErases) {
    int_set.insert(1);
    int_set.insert(2);
    int_set.insert(3);

    int_set.erase(2);
    EXPECT_EQ(int_set.size(), 2);

    int_set.erase(1);
    EXPECT_EQ(int_set.size(), 1);

    int_set.erase(3);
    EXPECT_EQ(int_set.size(), 0);
    EXPECT_TRUE(int_set.empty());
}

// ============================================================================
// Clear Tests
// ============================================================================

TEST_F(SparseSetTest, ClearEmptySet) {
    int_set.clear();
    EXPECT_EQ(int_set.size(), 0);
    EXPECT_TRUE(int_set.empty());
}

TEST_F(SparseSetTest, ClearNonEmptySet) {
    int_set.insert(1);
    int_set.insert(2);
    int_set.insert(3);

    int_set.clear();
    EXPECT_EQ(int_set.size(), 0);
    EXPECT_TRUE(int_set.empty());
    EXPECT_FALSE(int_set.contains(1));
    EXPECT_FALSE(int_set.contains(2));
    EXPECT_FALSE(int_set.contains(3));
}

TEST_F(SparseSetTest, ClearAndReinsert) {
    int_set.insert(1);
    int_set.insert(2);
    int_set.clear();

    auto [it, inserted] = int_set.insert(3);
    EXPECT_TRUE(inserted);
    EXPECT_EQ(int_set.size(), 1);
    EXPECT_TRUE(int_set.contains(3));
}

// ============================================================================
// Iterator Tests
// ============================================================================

TEST_F(SparseSetTest, IteratorBeginEnd) {
    int_set.insert(1);
    int_set.insert(2);
    int_set.insert(3);

    std::vector<int> values;
    for (auto it = int_set.begin(); it != int_set.end(); ++it) {
        values.push_back(*it);
    }

    EXPECT_EQ(values.size(), 3);
}

TEST_F(SparseSetTest, IteratorRangeBasedFor) {
    int_set.insert(10);
    int_set.insert(20);
    int_set.insert(30);

    std::vector<int> values;
    for (const auto &val : int_set) {
        values.push_back(val);
    }

    EXPECT_EQ(values.size(), 3);
    EXPECT_TRUE(std::find(values.begin(), values.end(), 10) != values.end());
    EXPECT_TRUE(std::find(values.begin(), values.end(), 20) != values.end());
    EXPECT_TRUE(std::find(values.begin(), values.end(), 30) != values.end());
}

TEST_F(SparseSetTest, IteratorEmptySet) {
    EXPECT_EQ(int_set.begin(), int_set.end());
}

TEST_F(SparseSetTest, ConstIterator) {
    int_set.insert(1);
    int_set.insert(2);

    const auto &const_set = int_set;

    std::vector<int> values;
    for (auto it = const_set.begin(); it != const_set.end(); ++it) {
        values.push_back(*it);
    }

    EXPECT_EQ(values.size(), 2);
}

TEST_F(SparseSetTest, IteratorCBeginCEnd) {
    int_set.insert(1);
    int_set.insert(2);

    std::vector<int> values;
    for (auto it = int_set.cbegin(); it != int_set.cend(); ++it) {
        values.push_back(*it);
    }

    EXPECT_EQ(values.size(), 2);
}

TEST_F(SparseSetTest, ReverseIterator) {
    int_set.insert(1);
    int_set.insert(2);
    int_set.insert(3);

    std::vector<int> values;
    for (auto it = int_set.rbegin(); it != int_set.rend(); ++it) {
        values.push_back(*it);
    }

    EXPECT_EQ(values.size(), 3);
    EXPECT_EQ(values[0], 3);
    EXPECT_EQ(values[1], 2);
    EXPECT_EQ(values[2], 1);
}

TEST_F(SparseSetTest, ConstReverseIterator) {
    int_set.insert(1);
    int_set.insert(2);
    int_set.insert(3);

    const auto &const_set = int_set;

    std::vector<int> values;
    for (auto it = const_set.rbegin(); it != const_set.rend(); ++it) {
        values.push_back(*it);
    }

    EXPECT_EQ(values.size(), 3);
}

TEST_F(SparseSetTest, IteratorCRBeginCREnd) {
    int_set.insert(1);
    int_set.insert(2);

    std::vector<int> values;
    for (auto it = int_set.crbegin(); it != int_set.crend(); ++it) {
        values.push_back(*it);
    }

    EXPECT_EQ(values.size(), 2);
}

TEST_F(SparseSetTest, IteratorModifyElements) {
    int_set.insert(1);
    int_set.insert(2);
    int_set.insert(3);

    for (auto &val : int_set) {
        val *= 10;
    }

    std::vector<int> values;
    for (const auto &val : int_set) {
        values.push_back(val);
    }

    EXPECT_TRUE(std::find(values.begin(), values.end(), 10) != values.end());
    EXPECT_TRUE(std::find(values.begin(), values.end(), 20) != values.end());
    EXPECT_TRUE(std::find(values.begin(), values.end(), 30) != values.end());
}

TEST_F(SparseSetTest, IteratorAfterErase) {
    int_set.insert(1);
    int_set.insert(2);
    int_set.insert(3);
    int_set.insert(4);

    int_set.erase(2);

    std::vector<int> values;
    for (const auto &val : int_set) {
        values.push_back(val);
    }

    EXPECT_EQ(values.size(), 3);
    EXPECT_TRUE(std::find(values.begin(), values.end(), 2) == values.end());
}

TEST_F(SparseSetTest, IteratorWithStdAlgorithms) {
    int_set.insert(5);
    int_set.insert(2);
    int_set.insert(8);
    int_set.insert(1);
    int_set.insert(9);

    auto count = std::count_if(int_set.begin(), int_set.end(), [](int x) { return x > 5; });
    EXPECT_EQ(count, 2);

    auto it = std::find_if(int_set.begin(), int_set.end(), [](int x) { return x == 8; });
    EXPECT_NE(it, int_set.end());
    EXPECT_EQ(*it, 8);

    int sum = std::accumulate(int_set.begin(), int_set.end(), 0);
    EXPECT_EQ(sum, 25);
}

TEST_F(SparseSetTest, IteratorStdDistance) {
    int_set.insert(1);
    int_set.insert(2);
    int_set.insert(3);

    auto dist = std::distance(int_set.begin(), int_set.end());
    EXPECT_EQ(dist, 3);
}

// ============================================================================
// Rehashing Tests
// ============================================================================

TEST_F(SparseSetTest, RehashingOnInsert) {
    // Initial size is 64, load factor is 0.7
    // Should rehash at floor(64 * 0.7) = 44 elements
    for (int i = 0; i < 100; ++i) {
        int_set.insert(i);
    }

    EXPECT_EQ(int_set.size(), 100);

    for (int i = 0; i < 100; ++i) {
        EXPECT_TRUE(int_set.contains(i));
    }
}

TEST_F(SparseSetTest, RehashManual) {
    int_set.insert(1);
    int_set.insert(2);
    int_set.insert(3);

    int_set.rehash(128);

    EXPECT_EQ(int_set.size(), 3);
    EXPECT_TRUE(int_set.contains(1));
    EXPECT_TRUE(int_set.contains(2));
    EXPECT_TRUE(int_set.contains(3));
}

TEST_F(SparseSetTest, RehashPreservesElements) {
    std::vector<int> values = {1, 65, 129, 193, 257, 321};

    for (int val : values) {
        int_set.insert(val);
    }

    for (int i = 1000; i < 1100; ++i) {
        int_set.insert(i);
    }

    for (int val : values) {
        EXPECT_TRUE(int_set.contains(val));
    }
}

// ============================================================================
// Reserve Tests
// ============================================================================

TEST_F(SparseSetTest, ReserveCapacity) {
    int_set.reserve(100);

    for (int i = 0; i < 50; ++i) {
        int_set.insert(i);
    }

    EXPECT_EQ(int_set.size(), 50);
    for (int i = 0; i < 50; ++i) {
        EXPECT_TRUE(int_set.contains(i));
    }
}

// ============================================================================
// Robin Hood Hashing Collision Tests
// ============================================================================

TEST_F(SparseSetTest, HashCollisionHandling) {
    int_set.insert(0);
    int_set.insert(64);
    int_set.insert(128);

    EXPECT_EQ(int_set.size(), 3);
    EXPECT_TRUE(int_set.contains(0));
    EXPECT_TRUE(int_set.contains(64));
    EXPECT_TRUE(int_set.contains(128));
}

TEST_F(SparseSetTest, CollisionWithErase) {
    int_set.insert(0);
    int_set.insert(64);
    int_set.insert(128);
    int_set.insert(192);

    int_set.erase(64);

    EXPECT_TRUE(int_set.contains(0));
    EXPECT_FALSE(int_set.contains(64));
    EXPECT_TRUE(int_set.contains(128));
    EXPECT_TRUE(int_set.contains(192));
}

// ============================================================================
// Stress Tests
// ============================================================================

TEST_F(SparseSetTest, LargeNumberOfInserts) {
    const int N = 10000;
    for (int i = 0; i < N; ++i) {
        int_set.insert(i);
    }

    EXPECT_EQ(int_set.size(), N);

    for (int i = 0; i < N; ++i) {
        EXPECT_TRUE(int_set.contains(i));
    }
}

TEST_F(SparseSetTest, InterleavedInsertAndErase) {
    for (int i = 0; i < 100; ++i) {
        int_set.insert(i);
    }

    for (int i = 0; i < 100; i += 2) {
        int_set.erase(i);
    }

    EXPECT_EQ(int_set.size(), 50);

    for (int i = 0; i < 100; ++i) {
        if (i % 2 == 0) {
            EXPECT_FALSE(int_set.contains(i));
        } else {
            EXPECT_TRUE(int_set.contains(i));
        }
    }
}

TEST_F(SparseSetTest, RandomOperations) {
    std::unordered_set<int> reference;

    for (int i = 0; i < 1000; ++i) {
        int val = i % 200;

        if (i % 3 == 0) {
            auto [it, sparse_result] = int_set.insert(val);
            bool ref_result          = reference.insert(val).second;
            EXPECT_EQ(sparse_result, ref_result);
        } else if (i % 3 == 1) {
            EXPECT_EQ(int_set.contains(val), reference.count(val) > 0);
        } else {
            size_t sparse_result = int_set.erase(val);
            size_t ref_result    = reference.erase(val);
            EXPECT_EQ(sparse_result, ref_result);
        }
    }

    EXPECT_EQ(int_set.size(), reference.size());
}

// ============================================================================
// Edge Cases
// ============================================================================

TEST_F(SparseSetTest, InsertEraseInsertSameValue) {
    int_set.insert(42);
    int_set.erase(42);
    auto [it, inserted] = int_set.insert(42);

    EXPECT_TRUE(inserted);
    EXPECT_TRUE(int_set.contains(42));
    EXPECT_EQ(int_set.size(), 1);
}

TEST_F(SparseSetTest, MultipleErasesOfSameValue) {
    int_set.insert(42);
    EXPECT_EQ(int_set.erase(42), 1);
    EXPECT_EQ(int_set.erase(42), 0);
    EXPECT_EQ(int_set.erase(42), 0);
}

TEST_F(SparseSetTest, EmptyAfterClearingAllElements) {
    for (int i = 0; i < 10; ++i) {
        int_set.insert(i);
    }

    for (int i = 0; i < 10; ++i) {
        int_set.erase(i);
    }

    EXPECT_EQ(int_set.size(), 0);
    EXPECT_TRUE(int_set.empty());

    for (int i = 0; i < 10; ++i) {
        EXPECT_FALSE(int_set.contains(i));
    }
}

TEST_F(SparseSetTest, InsertAfterClear) {
    int_set.insert(1);
    int_set.insert(2);
    int_set.clear();
    int_set.insert(3);

    EXPECT_EQ(int_set.size(), 1);
    EXPECT_TRUE(int_set.contains(3));
    EXPECT_FALSE(int_set.contains(1));
    EXPECT_FALSE(int_set.contains(2));
}

// ============================================================================
// Custom Types Tests
// ============================================================================

struct CustomType {
    int         x;
    std::string s;

    bool operator==(const CustomType &other) const { return x == other.x && s == other.s; }
};

template <>
struct std::hash<CustomType> {
    size_t operator()(const CustomType &ct) const {
        return std::hash<int>{}(ct.x) ^ std::hash<std::string>{}(ct.s);
    }
};

TEST(SparseSetCustomTypeTest, CustomTypeOperations) {
    sparse_set<CustomType> custom_set;

    CustomType obj1{42, "hello"};
    CustomType obj2{99, "world"};
    CustomType obj3{42, "hello"}; // Same as obj1

    auto [it1, ins1] = custom_set.insert(obj1);
    auto [it2, ins2] = custom_set.insert(obj2);
    auto [it3, ins3] = custom_set.insert(obj3);

    EXPECT_TRUE(ins1 && ins2);
    EXPECT_FALSE(ins3);

    EXPECT_EQ(custom_set.size(), 2);
    EXPECT_TRUE(custom_set.contains(obj1));
    EXPECT_TRUE(custom_set.contains(obj2));
    EXPECT_TRUE(custom_set.contains(obj3));

    EXPECT_EQ(custom_set.erase(obj1), 1);
    EXPECT_FALSE(custom_set.contains(obj1));
    EXPECT_EQ(custom_set.size(), 1);
}

TEST(SparseSetCustomTypeTest, CustomTypeIteration) {
    sparse_set<CustomType> custom_set;

    custom_set.insert({1, "a"});
    custom_set.insert({2, "b"});
    custom_set.insert({3, "c"});

    std::vector<CustomType> values;
    for (const auto &obj : custom_set) {
        values.push_back(obj);
    }

    EXPECT_EQ(values.size(), 3);
}

TEST(SparseSetCustomTypeTest, CustomTypeFind) {
    sparse_set<CustomType> custom_set;

    CustomType obj1{10, "test"};
    CustomType obj2{20, "data"};

    custom_set.insert(obj1);
    custom_set.insert(obj2);

    auto it = custom_set.find(obj1);
    EXPECT_NE(it, custom_set.end());
    EXPECT_EQ(it->x, 10);
    EXPECT_EQ(it->s, "test");

    CustomType obj3{99, "missing"};
    auto       it2 = custom_set.find(obj3);
    EXPECT_EQ(it2, custom_set.end());
}

// ============================================================================
// Performance Characteristics Tests
// ============================================================================

TEST_F(SparseSetTest, DensePackingProperty) {
    // Insert elements
    for (int i = 0; i < 10; ++i) {
        int_set.insert(i);
    }

    // Erase some elements
    int_set.erase(2);
    int_set.erase(5);
    int_set.erase(7);

    // Remaining elements should still be densely packed
    EXPECT_EQ(int_set.size(), 7);

    // Iteration should cover exactly size() elements
    int count = 0;
    for (const auto &val : int_set) {
        (void)val;
        count++;
    }
    EXPECT_EQ(count, 7);
}

TEST_F(SparseSetTest, IterationOrderPreservesInsertionAfterSwap) {
    int_set.insert(1);
    int_set.insert(2);
    int_set.insert(3);
    int_set.insert(4);
    int_set.insert(5);

    // Store original order
    std::vector<int> before;
    for (const auto &val : int_set) {
        before.push_back(val);
    }

    // Erase middle element (will swap with last)
    int_set.erase(3);

    std::vector<int> after;
    for (const auto &val : int_set) {
        after.push_back(val);
    }

    // Size should decrease
    EXPECT_EQ(after.size(), before.size() - 1);

    // Element 3 should not be present
    EXPECT_TRUE(std::find(after.begin(), after.end(), 3) == after.end());
}

// ============================================================================
// Boundary Condition Tests
// ============================================================================

TEST_F(SparseSetTest, InsertAtLoadFactorBoundary) {
    // Insert exactly up to load factor threshold
    int threshold = static_cast<int>(std::floor(64 * 0.7));

    for (int i = 0; i < threshold; ++i) {
        int_set.insert(i);
    }

    EXPECT_EQ(int_set.size(), threshold);

    // Next insert should trigger rehash
    int_set.insert(threshold);
    EXPECT_EQ(int_set.size(), threshold + 1);

    // All elements should still be accessible
    for (int i = 0; i <= threshold; ++i) {
        EXPECT_TRUE(int_set.contains(i));
    }
}

TEST_F(SparseSetTest, MaxIntValue) {
    int max_val = std::numeric_limits<int>::max();
    int min_val = std::numeric_limits<int>::min();

    int_set.insert(max_val);
    int_set.insert(min_val);

    EXPECT_TRUE(int_set.contains(max_val));
    EXPECT_TRUE(int_set.contains(min_val));

    EXPECT_EQ(int_set.erase(max_val), 1);
    EXPECT_FALSE(int_set.contains(max_val));
    EXPECT_TRUE(int_set.contains(min_val));
}

TEST_F(SparseSetTest, EmptyStringInsertion) {
    str_set.insert("");
    str_set.insert("a");
    str_set.insert("");

    EXPECT_EQ(str_set.size(), 2);
    EXPECT_TRUE(str_set.contains(""));
    EXPECT_TRUE(str_set.contains("a"));
}

TEST_F(SparseSetTest, LongStringInsertion) {
    std::string long_str(10000, 'x');
    str_set.insert(long_str);

    EXPECT_TRUE(str_set.contains(long_str));
    EXPECT_EQ(str_set.size(), 1);
}

// ============================================================================
// Sequential Operations Tests
// ============================================================================

TEST_F(SparseSetTest, AlternatingInsertErase) {
    for (int i = 0; i < 100; ++i) {
        int_set.insert(i);
        if (i > 0) {
            int_set.erase(i - 1);
        }
    }

    // Should only contain the last element
    EXPECT_EQ(int_set.size(), 1);
    EXPECT_TRUE(int_set.contains(99));
}

TEST_F(SparseSetTest, InsertEraseAllReinsert) {
    // Insert 50 elements
    for (int i = 0; i < 50; ++i) {
        int_set.insert(i);
    }

    // Erase all
    for (int i = 0; i < 50; ++i) {
        int_set.erase(i);
    }

    EXPECT_EQ(int_set.size(), 0);

    // Reinsert same elements
    for (int i = 0; i < 50; ++i) {
        int_set.insert(i);
    }

    EXPECT_EQ(int_set.size(), 50);
    for (int i = 0; i < 50; ++i) {
        EXPECT_TRUE(int_set.contains(i));
    }
}

TEST_F(SparseSetTest, MultipleRehashes) {
    // Force multiple rehashes
    for (int i = 0; i < 1000; ++i) {
        int_set.insert(i);
    }

    EXPECT_EQ(int_set.size(), 1000);

    // Verify all elements
    for (int i = 0; i < 1000; ++i) {
        EXPECT_TRUE(int_set.contains(i));
    }
}

// ============================================================================
// Copy/Move Semantics Tests
// ============================================================================

TEST_F(SparseSetTest, CopyThenModifyOriginal) {
    int_set.insert(1);
    int_set.insert(2);

    sparse_set<int> copy = int_set;

    int_set.insert(3);
    int_set.erase(1);

    EXPECT_EQ(int_set.size(), 2);
    EXPECT_EQ(copy.size(), 2);

    EXPECT_FALSE(int_set.contains(1));
    EXPECT_TRUE(copy.contains(1));

    EXPECT_TRUE(int_set.contains(3));
    EXPECT_FALSE(copy.contains(3));
}

TEST_F(SparseSetTest, MovePreservesElements) {
    int_set.insert(1);
    int_set.insert(2);
    int_set.insert(3);

    sparse_set<int> moved = std::move(int_set);

    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(moved.contains(1));
    EXPECT_TRUE(moved.contains(2));
    EXPECT_TRUE(moved.contains(3));
}

TEST_F(SparseSetTest, SelfAssignment) {
    int_set.insert(1);
    int_set.insert(2);

    int_set = int_set;

    EXPECT_EQ(int_set.size(), 2);
    EXPECT_TRUE(int_set.contains(1));
    EXPECT_TRUE(int_set.contains(2));
}

// ============================================================================
// Integration Tests
// ============================================================================

TEST_F(SparseSetTest, ComplexWorkflow) {
    // Insert initial data
    for (int i = 0; i < 20; ++i) {
        int_set.insert(i);
    }

    // Find and verify
    for (int i = 0; i < 20; ++i) {
        auto it = int_set.find(i);
        EXPECT_NE(it, int_set.end());
    }

    // Erase even numbers
    for (int i = 0; i < 20; i += 2) {
        int_set.erase(i);
    }

    EXPECT_EQ(int_set.size(), 10);

    // Verify odd numbers remain
    for (int i = 1; i < 20; i += 2) {
        EXPECT_TRUE(int_set.contains(i));
    }

    // Clear and refill
    int_set.clear();
    for (int i = 100; i < 110; ++i) {
        int_set.insert(i);
    }

    EXPECT_EQ(int_set.size(), 10);
    EXPECT_TRUE(int_set.contains(105));
    EXPECT_FALSE(int_set.contains(5));
}

TEST_F(SparseSetTest, SetOperationsSimulation) {
    sparse_set<int> set_a;
    sparse_set<int> set_b;

    // Populate sets
    for (int i = 0; i < 10; ++i) {
        set_a.insert(i);
    }
    for (int i = 5; i < 15; ++i) {
        set_b.insert(i);
    }

    // Find intersection
    std::vector<int> intersection;
    for (const auto &val : set_a) {
        if (set_b.contains(val)) {
            intersection.push_back(val);
        }
    }

    EXPECT_EQ(intersection.size(), 5); // 5, 6, 7, 8, 9

    // Verify intersection values
    for (int i = 5; i < 10; ++i) {
        EXPECT_TRUE(std::find(intersection.begin(), intersection.end(), i) != intersection.end());
    }
}

// ============================================================================
// Main
// ============================================================================

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#ifdef __GNUC__
#    pragma GCC diagnostic pop
#endif

#ifdef __clang__
#    pragma clang diagnostic pop
#endif

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
