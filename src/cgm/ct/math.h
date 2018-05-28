/**@file
 * @brief Compile Time evaluated math functions.
 *
 *
 */
#pragma once
#ifndef CGM_CT_MATH_H_INCLUDED
#define CGM_CT_MATH_H_INCLUDED

namespace cgm::ct::math {

/** Compile Time factorial.
 *
 * Recursive implementation is OK for CT evaluation.
 *
 * Example:
 *
 * ```cpp
 * const unsigned long fact3{ factorial(3) };
 * constexpr long long fact4{ factorial(4) };
 * ```
 */
template<typename T>
// where T is numeric
constexpr unsigned int factorial(T i)
{
  return (i == 0) ?             // terminal condition
         1u :                   // and terminal value
         i * factorial(i - 1u); // recursive definition
}


/// Compile Time logarithm.
///
template<typename T>
// where T is numeric
constexpr T log2(T n, T p = 0) {
    return (n <= 1) ? p : log2(n / 2, p + 1);
}

constexpr void unit_test_log2() {
    static_assert (log2(64) == 6, "1,2,4,8,16,32,64; 2^6=64");
}

} // cgm::ct::math
#endif
