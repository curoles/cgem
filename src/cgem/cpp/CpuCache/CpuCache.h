/**@file
 * @brief     CPU cache SW model
 * @author    Igor Lesik 2010-2014
 * @copyright 2010-2014 Igor Lesik
 *
 *  License:   Distributed under the Boost Software License, Version 1.0.
 *            (See http://www.boost.org/LICENSE_1_0.txt)
 *
 * @page page_CPU_cache CPU Cache
 *
 * @section section_Cache_structure Cache structure
 * <table> <caption>Cache structure</caption>
 * <tr><th><th>Way 0<th> Way 1<th>...<th>Way N
 * <tr><td>Set 0<td>entry<td>entry<td>entry<td>entry
 * <tr><td>Set 1<td>entry<td>entry<td>entry<td>entry
 * <tr><td>...
 * <tr><td>Set M<td>entry<td>entry<td>entry<td>entry
 * </table>
 *
 * Cache entry usually has the following structure:
 * <table>
 * <tr><td>Tag<td>Data Block<td>Flag bits
 * </table>
 * The <em>Data Block</em> (cache line) contains the actual data
 * fetched from the main memory.
 * The <em>Tag</em> contains part of the address of the actual data fetched
 * from the main memory.
 *
 * An effective memory address is split (MSB to LSB) into the tag,
 * the index and the block offset.
 * The index describes which cache row/set that the data has been put in.
 * The index length is log_2(r) bits for r cache rows.
 */
#pragma once

#include <array>
#include <tuple>
#include <functional>

/** cgem*/namespace cgem { /** cpu*/namespace cpu {

/// Get number of bytes to hold given number of bits.
///
constexpr size_t bit2byte(
    size_t bits ///< number of bits
)
{
    return (bits+7)/8;
}

constexpr bool unit_test_bit2byte(){
    static_assert (bit2byte(8) == 1, "8 bits = 1 byte");
    static_assert (bit2byte(15) == 2, "15 bits = 2 bytes");
    return true;
}

/// Get number of words to hold given number of bits.
///
constexpr size_t bit2word(
    size_t bits, ///< number of bits
    size_t wsz   ///< word size
) {
    return (bit2byte(bits)+(wsz-1))/wsz;
}

constexpr bool unit_test_bit2word(){
    static_assert (
        bit2word(64, sizeof(uint32_t)) == 2, "64 bits = 2 32bit words"
    );
    return true;
}

/// Compile Time logarithm.
///
constexpr unsigned Log2(unsigned n, unsigned p = 0) {
    return (n <= 1) ? p : Log2(n / 2, p + 1);
}

constexpr bool unit_test_Log2(){
    static_assert (
        Log2(64) == 6, "1,2,4,8,16,32,64; 2^6=64"
    );
    return true;
}

/// Compile Time bit mask generation, MSB pos 2 => 0b111.
///
constexpr unsigned makeBitMask(
    unsigned pos ///< MSBit position
)
{
    return (pos == 0) ? 0x1 : ((0x1 << pos) | makeBitMask(pos - 1));
}

constexpr bool unit_test_makeBitMask(){
    static_assert (makeBitMask(0) == 0b1, "b1");
    static_assert (makeBitMask(1) == 0b11, "b11");
    static_assert (makeBitMask(9) == 0b1111111111, "b1111111111");
    return true;
}

template<class TCache>
size_t defaultCacheEvictionPolicy(const TCache& cache, size_t setId)
{
    size_t wayId {0};
    for (wayId = 0; wayId < TCache::numberWays; ++wayId) {
        if (not cache.getLine(setId, wayId).isValid()) return wayId;
    }
    return 0;
}

/** SW model of HW/RTL CPU cache.
 *
 * Entry structure: tag + data_block + flag_bits
 * Address split: tag + index + block_offset
 */
template<
    typename TAddr,          ///< address type, uint64_t is ok
    typename TWord,          ///< word type defines word size, from uint8_t to uint64_t
    size_t kNumberLines,     ///< number of cache lines
    size_t kNumberWays,      ///< number of ways or associativity
    size_t kTagSize,         ///< size of Tag bits in cache line
    size_t kDataSize,        ///< size of Data bits in cache line
    size_t kFlagsSize   = 1, ///< size of flags/info bits, at least 1 for Valid bit
    size_t kLineValidBit= 0  ///< chache line validity bit
>
class Cache
{
public:
    typedef TAddr Addr;
    typedef TWord Word;
    static const size_t numberLines {kNumberLines};
    static const size_t numberWays {kNumberWays};

    template<size_t sizeBits> using WordArray = std::array<Word, bit2word(sizeBits,sizeof(Word))>;

    struct Line
    {
        static const size_t sizeBits = kTagSize + kDataSize + kFlagsSize;

        struct Tag
        {
            static const size_t sizeBits = kTagSize;
            typedef uint64_t ValType;
            ValType val;
        } tag;

        struct Data : public WordArray<kDataSize>
        {
            static const size_t sizeBits = kDataSize;
            static const size_t sizeBytes = bit2byte(sizeBits);
            static const size_t sizeWords = bit2word(sizeBits,sizeof(Word));
            static const size_t addrBits = cpu::Log2(sizeBytes);

            constexpr Word bitN(size_t n) {return 1u << n;}
            bool bitAt(size_t n) const {return WordArray<kDataSize>::at(bit2word(n,sizeof(Word))) & bitN(n);}
            void setBit(size_t n) {WordArray<kDataSize>::at(bit2word(n,sizeof(Word))) |= bitN(n);}
            void clearBit(size_t n) {WordArray<kDataSize>::at(bit2word(n,sizeof(Word))) &= ~bitN(n);}
        } data;

        struct Flags
        {
            static const size_t sizeBits = kFlagsSize;
            typedef uint64_t ValType;
            ValType val;
            constexpr ValType bitN(size_t n) {return 1lu << n;}
            bool bitAt(size_t n) const {return val & bitN(n);}
            void setBit(size_t n) {val |= bitN(n);}
            void clearBit(size_t n) {val &= ~bitN(n);}
        } flags;

        bool isValid() const {return flags.bitAt(kLineValidBit);}
        void setValid() {flags.setBit(kLineValidBit);}
        void clearValid() {flags.clearBit(kLineValidBit);}
        void invalidate() {clearValid();}

        static
        typename Tag::ValType getAddrTag(Addr addr) {
            return (addr >> (Data::addrBits + indexAddrBits)) & tagAddrMask;
        }

        static size_t getAddrByteId(Addr addr) { return addr & dataAddrMask; }
        static size_t getAddrWordId(Addr addr) { return getAddrByteId(addr)/sizeof(Word); }

        bool isAddrTagMatch(Addr addr) const {
            return getAddrTag(addr) == tag.val;
        }

    };

    static const size_t indexAddrBits = cpu::Log2(kNumberLines);
    static const size_t addrBits = Line::Data::addrBits + Line::Tag::sizeBits + indexAddrBits;

    static const unsigned dataAddrMask = cpu::makeBitMask(Line::Data::addrBits - 1);
    static const unsigned indexAddrMask = cpu::makeBitMask(indexAddrBits - 1);
    static const unsigned tagAddrMask = cpu::makeBitMask(Line::Tag::sizeBits - 1);


    using Lines = std::array<Line,kNumberLines * kNumberWays>;

    Lines lines;

    size_t hitCount, missCount;

    using EvictionPolicy = std::function<size_t(const Cache&,size_t)>;

    EvictionPolicy evictionPolicy;

public:
    Cache(const Cache& ) = delete;
    Cache(const Cache&& ) = delete;
    Cache(EvictionPolicy policy = cpu::defaultCacheEvictionPolicy<Cache>):
        hitCount(0), missCount(0), evictionPolicy(policy)
    {
        static_assert((sizeof(typename Line::Tag::ValType)/8) <= kTagSize, "tag size more than max");
        static_assert((sizeof(typename Line::Flags::ValType)/8) <= kFlagsSize, "flags size more than max");
        static_assert(kDataSize % sizeof(Word) == 0, "data size word aligned");
    }

    void invalidateAll() {
        for (auto& line : lines) {
            line.invalidate();
        }
    }

    Line& getLine(size_t setId, size_t wayId) {
        return lines.at(kNumberWays*setId + wayId);
    }

    const Line& getLine(size_t setId, size_t wayId) const {
        return lines.at(kNumberWays*setId + wayId);
    }

    size_t getLineIndexByAddr(Addr addr) {
        return (addr >> Line::Data::addrBits) & indexAddrMask;
    }

    void insertDataWord(size_t setId, size_t wayId, size_t wordId, Word word) {
        getLine(setId, wayId).data.at(wordId) = word;
    }

    Cache::Line& insertLine(Addr addr, const Cache::Line& newLine) {
        size_t setId { getLineIndexByAddr(addr) };
        size_t wayId { findWayToInsert(setId) };
        Line& line = getLine(setId, wayId);
        line = newLine;
        line.tag.val = Cache::Line::getAddrTag(addr);
        line.setValid();
        return line;
    }

    std::tuple<bool,size_t,size_t,size_t> findLine(Addr addr) {
        bool hit {false};
        size_t setId{getLineIndexByAddr(addr)}, wayId{777}, wordId{Line::getAddrWordId(addr)};

        for (wayId = 0; wayId < kNumberWays; ++wayId) {
            Line& line = getLine(setId, wayId);
            if (line.isValid() and line.isAddrTagMatch(addr)) {
                hit = true;
                break;
            }
        }

        if (hit) ++hitCount; else ++missCount;

        return std::tie(hit, setId, wayId, wordId);
    }

    void resetPerfCounters() {
        hitCount = missCount = 0;
    }

    size_t getHitCount() const {return hitCount;}
    size_t getMissCount() const {return missCount;}

    size_t findWayToInsert(size_t setId) {
        return evictionPolicy(*this, setId);
    }

};


}} // end of cgem::cpu


#ifdef GEM_CPU_CACHE_TEST


#include <cassert>
#include <stdexcept>
#include <iostream>

int test()
{
    const uint32_t tmp = cgem::cpu::makeBitMask(4);
    //std::cout << std::hex << "0x" << tmp << "\n";
    static_assert(tmp == 0x1F, "");

    const size_t kNumberLines {16};
    const size_t kNumberWays {2};
    const size_t kTagSize {2};
    const size_t kDataSize {64};
    const size_t kFlagsSize {1};

    using Cache =
        cgem::cpu::Cache<
            uint64_t,     // 64 bit address
            uint32_t,     // 32 bit word
            kNumberLines,
            kNumberWays,
            kTagSize,
            kDataSize,
            kFlagsSize
        >;

    static_assert(Cache::Line::Tag::sizeBits == kTagSize, "tag size");
    static_assert(Cache::Line::Data::sizeBits == kDataSize, "data size");
    static_assert(Cache::Line::Data::sizeBytes == kDataSize/8, "data size");
    static_assert(Cache::Line::Data::sizeWords == kDataSize/(8*sizeof(Cache::Word)), "data size");
    static_assert(Cache::Line::Data::addrBits == 3, "addr bits");
    static_assert(Cache::Line::Flags::sizeBits == kFlagsSize, "flag size");

    Cache cache;
    assert(cache.numberLines == kNumberLines);
    static_assert(Cache::numberLines == kNumberLines, "# lines");
    assert(cache.numberWays == kNumberWays);
    static_assert(Cache::numberWays == kNumberWays, "# ways");

    cache.invalidateAll();

    bool hadException {false};
    try {cache.getLine(kNumberLines,0);}catch(std::out_of_range& e){hadException=true;}
    assert(hadException);

    std::cout << std::dec << "total # of addr bits:" << cache.addrBits << "\n";
    std::cout << std::dec << "# of data addr bits:" << Cache::Line::Data::addrBits << "\n";
    std::cout << std::dec << "# of index addr bits:" << Cache::indexAddrBits << "\n";

    size_t setId, wayId, wordId;
    Cache::Line line {{0xdead},{},{0xdead}};

    for (uint64_t addr = 0; addr < 0x1FF; addr+= 4) {
        //std::cout << std::hex << "insert addr=0x" << addr << "\n";
        line.data.at(0) = addr;
        line.data.at(1) = addr >> 32;
        cache.insertLine(addr, line);
    }

    for (uint64_t addr = 0; addr < 0x1FF; addr+= 4) {
        bool hit;
        std::tie(hit,setId,wayId,wordId) = cache.findLine(addr);
        if (hit) {
            Cache::Line& line = cache.getLine(setId, wayId);
            std::cout << std::dec << "set=" << setId << ", way=" << wayId << ", word=" << wordId << ": ";
            std::cout << std::hex << "addr=" << addr << ", data=" << line.data.at(wordId) << "\n";
        }
    }

    std::cout << std::dec << "hits=" << cache.getHitCount() << ", misses=" << cache.getMissCount() << "\n";

    return 0;
}

#endif
