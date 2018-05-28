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
 *
 * |     | Way 0 | Way 1 | Way 2 | ... | Way N |
 * | --- | ----- | ----- | ----- | --- | ----- |
 * |Set 0| entry | entry | entry | ... | entry |
 * |Set 1| entry | entry | entry | ... | entry |
 * | ... | ...   | ...   | ...   | ... | ...   |
 * |Set M| entry | entry | entry | ... | entry |
 *
 *
 * Cache entry usually has the following structure:
 * 
 * | Tag | Data Block | Flag bits |
 * | --- | ---------- | --------- |
 * |     |            |           |
 * 
 * The <em>Data Block</em> (cache line) contains the actual data
 * fetched from the main memory.
 * The <em>Tag</em> contains part of the address of the actual data fetched
 * from the main memory.
 *
 * An effective memory address is split (MSB to LSB) into the tag,
 * the index and the block offset.
 * 
 * | Tag | Index | Block offset |
 * | --- | ----- | ------------ |
 * |     |       |              |
 *
 * The index describes which cache row/set that the data has been put in.
 * The index length is log_2(r) bits for r cache rows.
 */
#pragma once
#ifndef CGM_MODEL_CPU_CACHE_H_INCLUDED
#define CGM_MODEL_CPU_CACHE_H_INCLUDED

#include <array>
#include <tuple>
#include <functional>

#include "cgm/ct/bits.h"
#include "cgm/ct/math.h"
#include "cgm/numrange.h"

namespace cgm::model::cpu {

using namespace cgm::ct;

template<class TCache>
size_t defaultCacheEvictionPolicy(const TCache& cache, size_t setId)
{
    for (auto wayId : num::range(TCache::numberWays)) {
        auto line = cache.getLine(setId, wayId); 
        if (not line.isValid()) return wayId;
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

    template<size_t sizeBits>
    using WordArray = std::array<Word, bits::words(sizeBits,sizeof(Word))>;

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
            static const size_t sizeBytes = bits::bytes(sizeBits);
            static const size_t sizeWords = bits::words(sizeBits,sizeof(Word));
            static const size_t addrBits = cgm::ct::math::log2(sizeBytes);

            constexpr Word bitN(size_t n) {return 1u << n;}
            bool bitAt(size_t n) const {return WordArray<kDataSize>::at(bits::words(n,sizeof(Word))) & bitN(n);}
            void setBit(size_t n) {WordArray<kDataSize>::at(bits::words(n,sizeof(Word))) |= bitN(n);}
            void clearBit(size_t n) {WordArray<kDataSize>::at(bits::words(n,sizeof(Word))) &= ~bitN(n);}
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

    static const size_t indexAddrBits = cgm::ct::math::log2(kNumberLines);
    static const size_t addrBits = Line::Data::addrBits + Line::Tag::sizeBits + indexAddrBits;

    static const unsigned dataAddrMask = bits::make1s(Line::Data::addrBits - 1);
    static const unsigned indexAddrMask = bits::make1s(indexAddrBits - 1);
    static const unsigned tagAddrMask = bits::make1s(Line::Tag::sizeBits - 1);


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


} // cgm::model::cpu

#endif
