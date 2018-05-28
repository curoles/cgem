/**@file
 * @brief Compile Time evaluated bit manipulation functions.
 *
 *
 */
#pragma once
#ifndef CGM_CT_BITS_H_INCLUDED
#define CGM_CT_BITS_H_INCLUDED

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

} // cgm::ct::bits

#endif
