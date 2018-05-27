/**@file
 * @brief Test Compile Time evaluated math functions.
 *
 *
 */
#include "gtest/gtest.h"
#include "cgm/ct/math.h"

using namespace cgm::ct::math;

/// Test factorial
///
/// @test
///
TEST (CTMath, Factorial) {
    const unsigned long fact3{factorial(3)};
    constexpr long long fact4{factorial(4)};
    static_assert(factorial(9) == 1*2*3*4*5*6*7*8*9);
    ASSERT_EQ (factorial(9), 1*2*3*4*5*6*7*8*9);
}

