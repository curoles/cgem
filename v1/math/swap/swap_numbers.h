//gcc test.cpp -DUNIT_TEST -std=c++11
//gcc test.c -DUNIT_TEST

#pragma once

#ifdef __cplusplus

#include <cstdint>
#include <cassert>

namespace cgem{ namespace math {

template<typename T>
void swap_nums(T& a, T& b)
{
    a ^= b; // a'=a^b
    b ^= a; // b'=b^a'=b^a^b=a
    a ^= b; // a''=a'^b'=a^b^a=b
}

#ifdef UNIT_TEST
void test_swap_nums()
{
    std::uint32_t n1 = 777, n2 = 666;
    cgem::math::swap_nums(n1, n2);
    assert(n1 == 666 && n2 == 777);

    std::int64_t i1 = 777, i2 = -666;
    cgem::math::swap_nums(i1, i2);
    assert(i1 == -666 && i2 == 777);
}
#endif

}} //cgem::math

#else // C, not C++

#include <stdint.h>
#ifdef UNIT_TEST
#include <assert.h>
#endif

#define cgem_math_(obj) cgem_math__##obj

INLINE_C
void cgem_math_(swap_nums_uint32)(
    uint32_t* a,
    uint32_t* b
)
{
    *a ^= *b; // a'=a^b
    *b ^= *a; // b'=b^a'=b^a^b=a
    *a ^= *b; // a''=a'^b'=a^b^a=b
}

#ifdef UNIT_TEST
INLINE_C
void cgem_math_(test_swap_nums_uint32)()
{
    uint32_t n1 = 777, n2 = 666;
    cgem_math_(swap_nums_uint32)(&n1, &n2);
    assert(n1 == 666 && n2 == 777);
}
#endif

INLINE_C
void cgem_math_(swap_nums_uint16)(
    uint16_t* a,
    uint16_t* b
)
{
    *a ^= *b; // a'=a^b
    *b ^= *a; // b'=b^a'=b^a^b=a
    *a ^= *b; // a''=a'^b'=a^b^a=b
}

INLINE_C
void cgem_math_(swap_halfs_uint32)(
    uint32_t* n
)
{
    cgem_math_(swap_nums_uint16)((uint16_t*)n, ((uint16_t*)n)+1);
}

#ifdef UNIT_TEST
INLINE_C
void cgem_math_(test_swap_halfs_uint32)()
{
    uint32_t n = 0xdeadbeef;
    cgem_math_(swap_halfs_uint32)(&n);
    assert(n == 0xbeefdead);
}
#endif

#undef cgem_math_

#endif // __cplusplus
