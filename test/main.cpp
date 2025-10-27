#include <gtest/gtest.h>
#include "sparse-set.hpp"

TEST(SparseSetTest, InsertAndContains) {
    sparse_set<int> set;
    EXPECT_FALSE(set.contains(1));
    set.insert(1);
    EXPECT_TRUE(set.contains(1));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
