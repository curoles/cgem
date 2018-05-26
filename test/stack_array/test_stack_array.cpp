/**@file
 * @brief Test `stack_array`.
 *
 *
 */
#include "gtest/gtest.h"
#include <numeric>
#include "cgm/stack_array.h"

/// Test creation of stack_array
///
TEST (StackArray, Creation) {
    int sz{10};
    stack_array(int, sz, a);
    ASSERT_EQ (a.size(), sz) << "Size must be 10";
    ASSERT_NE (a.data(), nullptr) << "Data not null";
}

/// Test operator[]
///
TEST (StackArray, SetGetElement) {
    stack_array(int, 10, a);
    a[3] = 3;
    ASSERT_EQ (a[3], 3) << "array[3] must be 3";
}

/// Test version make_unique<T[]>(size)
///
TEST (StackArray, DynamicMemoryAllocation) {
    CGM_STACK_ARRAY_UPTR(unsigned long, 10, a);
    ASSERT_EQ (a.size(), 10) << "Size must be 10";
    ASSERT_NE (a.data(), nullptr) << "Data not null";
    a[4] = a[3] = a[2] = 4;
    ASSERT_EQ (a[4], 4) << "array[4] must be 4";
}

/// Test iterators work with numeric algorithms and range `for`.
///
TEST (StackArray, Iterators) {
    int sz{10};
    stack_array(int, sz, a);
    std::fill(a.begin(), a.end(), 3);
    ASSERT_EQ (a[8], 3);
    std::iota(a.begin(), a.end(), -4);
    ASSERT_EQ (a[0], -4);
    ASSERT_EQ (a[1], -3);
    ASSERT_EQ (a[2], -2);
    ASSERT_EQ (a[3], -1);
    ASSERT_EQ (a[4],  0);
    ASSERT_TRUE(a[5] == 1 and a[9] == 5);
    int sum{0};
    for (auto elem : a) {
        sum += elem;
    }
    ASSERT_EQ(sum, std::accumulate(a.begin(), a.end(), 0));
}
