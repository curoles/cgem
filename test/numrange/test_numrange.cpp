/**@file
 * @brief Test Numeric Range.
 *
 *
 */
#include "gtest/gtest.h"
#include "cgm/numrange.h"

using namespace cgm::num;

/// Test size
///
/// @test
///
TEST (NumRange, Size) {
    auto r1 = range(0,5);
    ASSERT_EQ (r1.size(), 5);
    auto r2 = range(2,7,1);
    ASSERT_EQ (r2.size(), 5);
    auto r3 = range(2,8,2);
    ASSERT_EQ (r3.size(), 3);
    auto r4 = range(2,7,2);
    ASSERT_EQ (r4.size(), 3);
    auto r5 = range(2,3,0.1);
    ASSERT_EQ (r5.size(), 10);
    auto r6 = range(15,2,1);
    ASSERT_EQ (r6.size(), 13);
    auto r7 = range(15,2,-3);
    ASSERT_EQ (r7.size(), 5);
    auto r8 = range(3,2,0.1);
    ASSERT_EQ (r8.size(), 10);
    auto r9 = range(3,2,-0.2);
    ASSERT_EQ (r9.size(), 5);
}

/// Test index
///
/// @test
///
TEST (NumRange, Index) {
    auto r1 = range(0,5);
    ASSERT_EQ (r1[1], 1);
    ASSERT_EQ (range(5)[2], 2);
    auto r2 = range(2,7,1);
    ASSERT_EQ (r2[3], 5);
    auto r3 = range(2,8,2);
    ASSERT_EQ (r3[2], 6);
    auto r4 = range(2,7,2);
    ASSERT_EQ (r4[2], 6);
    auto r5 = range(2,3,0.1);
    ASSERT_EQ (r5[5], 2.5);
}

/// Test for loop
///
/// @test
///
TEST (NumRange, ForLoop) {
    int snoop{2};
    for (auto i : range(2,7,2)) {
        ASSERT_EQ (i, snoop);
        snoop += 2;
    }

    snoop = 0;
    for (auto i : range(17)) {
        ASSERT_EQ (i, snoop);
        snoop += 1;
    }

    auto fsnoop{2.3};
    for (auto f : range(2.3, 5.1, 0.134)) {
        ASSERT_EQ (f, fsnoop);
        fsnoop += 0.134;
    }
}

/// Test `for` loop for reversed range
///
/// @test
///
TEST (NumRange, ReversedRange) {
    int snoop{7};
    for (auto i : range(7,2,-2)) {
        ASSERT_EQ (i, snoop);
        snoop -= 2;
    }

    snoop = 17;
    for (auto i : range(17,7)) {
        ASSERT_EQ (i, snoop);
        snoop -= 1;
    }

    auto fsnoop{5.1};
    for (auto f : range(5.1, 2.3, -0.134)) {
        ASSERT_EQ (f, fsnoop);
        fsnoop -= 0.134;
    }
}
