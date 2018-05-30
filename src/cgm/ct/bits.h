/**@file
 * @brief Compile Time evaluated bit manipulation functions.
 *
 *
 */
#pragma once
#ifndef CGM_CT_BITS_H_INCLUDED
#define CGM_CT_BITS_H_INCLUDED

#include <type_traits>
#include <typeinfo>

namespace cgm::ct::bits {

/// Numbers of bits in one byte.
///
constexpr size_t in1byte = 8;

/// Get number of bytes to hold given number of bits.
///
constexpr size_t bytes (size_t nbits) {
    return (nbits + (bits::in1byte - 1)) / bits::in1byte;
}

constexpr auto unit_test_bytes() {
    static_assert (bytes(8) == 1, "8 bits == 1 byte");
    static_assert (bytes(15) == 2, "15 bits requires 2 bytes");
}

template<size_t bytes>
struct Bytes {
using type = std::conditional_t< (bytes > 4), uint64_t,
             std::conditional_t< (bytes > 2), uint32_t,
             std::conditional_t< (bytes > 1), uint16_t, uint8_t> > >;
};

constexpr auto unit_test_type() {
    static_assert (std::is_same<Bytes<1>::type,uint8_t>::value);
    static_assert (std::is_same<Bytes<3>::type,uint32_t>::value);
}


/// Get number of words to hold given number of bits.
///
constexpr size_t words (
    size_t nbits, ///< number of bits
    size_t wsz    ///< word size in bytes
) {
    return (bytes(nbits) + (wsz-1)) / wsz;
}

constexpr auto unit_test_words() {
    static_assert (words(64, sizeof(uint32_t)) == 2, "64 bits = 2 32bit words");
}

/// Compile Time bit mask 1's starting from position.
///
/// MSB pos 2 => 0b111.
///
constexpr unsigned make1s(
    unsigned pos ///< MSBit position
)
{
    return (pos == 0) ? 0x1 : ((0x1 << pos) | make1s(pos - 1));
}

constexpr auto unit_test_makeBitMask() {
    static_assert (make1s(0) == 0b1, "b1");
    static_assert (make1s(1) == 0b11, "b11");
    static_assert (make1s(9) == 0b1111111111, "b1111111111");
}

/// Get a number with a single bit set to 1 at certain position.
///
template<typename T>
constexpr T one(size_t pos) { return T{1} << pos; }

constexpr auto unit_test_one() {
    static_assert (one<uint8_t>(4) == 0b10000);
    static_assert (one<uint64_t>(10) == 0b10000000000);
}

/// Return non-zero if bit is set at certain position.
///
template<typename T>
constexpr
T at(T val, size_t pos) { return val & one<T>(pos); }

constexpr auto unit_test_at() {
    static_assert (at(0b10010,1) and at(0b10010,1));
    static_assert (at(0b101111100,7) == 0);
}

/// Set bit at certain position.
///
template<typename T>
constexpr
T set(T val, size_t pos) { return val | one<T>(pos);}

constexpr auto unit_test_set() {
    static_assert (set(0b110,0) == 7);
}

/// Clear bit at certain position.
///
template<typename T>
constexpr
T clear(T val, size_t pos) { return val & ~one<T>(pos); }

constexpr auto unit_test_clear() {
    static_assert (clear(0b111,2) == 3);
}

} // cgm::ct::bits

#endif
