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
        snoop += 0.134;
    }


}

