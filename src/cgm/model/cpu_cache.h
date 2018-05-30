/**@file
 * @brief     CPU cache SW model
 * @author    Igor Lesik 2010-2014
 * @copyright 2010-2014 Igor Lesik
 *
 *  License:   Distributed under the Boost Software License, Version 1.0.
 *            (See http://www.boost.org/LICENSE_1_0.txt)
 *
 * CPU Cache
 * =========
 *
 * Cache structure
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
 * The *Data Block* (cache line) contains the actual data
 * fetched from the main memory.
 * The *Tag* contains part of the address of the actual data fetched
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

namespace cache {

/// For certain Set, find available Way or make one available by eviction.
///
template<class TCache>
size_t simpleEvictionPolicy(const TCache& cache, size_t setId)
{
    for (auto wayId : num::range(cache.numWays())) {
        auto line = cache.getLine(setId, wayId); 
        if (not line.isValid()) return wayId;
    }
    return 0; // All ways are taken, use first one then.
}

template<typename T>
struct Tag
{
    const size_t size; // bits
    T val;
    using value_type = T;
};

template<typename T>
struct Flags
{
    const size_t size; // bits
    T val;
    using value_type = T;

    T bitAt(size_t pos) const {return bits::at(val,pos);}
    void setBit(size_t pos) {val = bits::set(val,pos);}
    void clearBit(size_t pos) {val = bits::clear(val,pos);}
};

/// Hand-made alternative to std::bitset, has no length limitation,
/// has by word access.
///
template<size_t sizeBits, typename TWord>
using WordArray = std::array<TWord, bits::words(sizeBits,sizeof(TWord))>;

template<size_t sizeBits, typename TWord>
struct Data : public WordArray<sizeBits, TWord>
{
    static constexpr size_t sizeBytes = bits::bytes(sizeBits);
    static constexpr size_t sizeBits_ = sizeBits;
    static constexpr size_t sizeWords = bits::words(sizeBits,sizeof(TWord));
    static constexpr size_t addrBits = cgm::ct::math::log2(sizeBytes);

    TWord& wordAtBit(size_t pos) {
        return WordArray<sizeBits,TWord>::at(bits::words(pos,sizeof(TWord)));
    }

    TWord bitAt(size_t pos) const {return bits::at(wordAtBit(pos),pos);}
    void setBit(size_t pos)   {wordAtBit(pos) |=  bits::one<TWord>(pos);}
    void clearBit(size_t pos) {wordAtBit(pos) &= ~bits::one<TWord>(pos);}
};

template<typename TAddr, typename TWord,
         size_t kTagSize, size_t kDataSize,
         size_t kFlagsSize, size_t kLineValidBit>
struct Line
{
    static constexpr size_t sizeBits = kTagSize + kDataSize + kFlagsSize;

    using Tag = cache::Tag<typename bits::Bytes<bits::bytes(kTagSize)>::type>;
    Tag tag{kTagSize,0};

    using Data = cache::Data<kDataSize,TWord>;
    Data data;

    using Flags = cache::Flags<typename bits::Bytes<bits::bytes(kFlagsSize)>::type>;
    Flags flags{kFlagsSize,0};

    bool isValid() const { return !!flags.bitAt(kLineValidBit); }
    void setValid()   { flags.setBit(kLineValidBit); }
    void clearValid() { flags.clearBit(kLineValidBit); }
    void invalidate() { clearValid(); }

    static constexpr TAddr dataAddrMask = bits::make1s(Data::addrBits - 1);

    static size_t getByteId(TAddr addr) { return addr & dataAddrMask; }
    static size_t getWordId(TAddr addr) { return getAddrByteId(addr)/sizeof(TWord); }
};

} // namespace cache

/** SW model of HW/RTL CPU cache.
 *
 * Entry structure: tag + data_block + flag_bits
 * Address split: tag + index + block_offset
 */
template<
    typename TAddr,          ///< address type, uint64_t is ok
    typename TWord,          ///< word type defines word size, from uint8_t to uint64_t
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
    using Line = cache::Line<TAddr, TWord, kTagSize, kDataSize, kFlagsSize, kLineValidBit>;
    using Lines = std::vector<Line>;
    using EvictionPolicy = std::function<size_t(const Cache&,size_t)>;
public:
    const size_t numberLines;
    const size_t numberWays;
    const size_t indexAddrBits;
    const size_t allAddrBits;
    static constexpr TAddr dataAddrMask = bits::make1s(Line::Data::addrBits - 1);
    const  TAddr indexAddrMask;
    static constexpr TAddr tagAddrMask = bits::make1s(Line::Tag::sizeBits - 1);
private:

    size_t hitCount, missCount;
    EvictionPolicy evictionPolicy;
    Lines lines;

public:
    Cache(const Cache& ) = delete;
    Cache(const Cache&& ) = delete;
    Cache(
        size_t numberLines,     ///< number of cache lines
        size_t numberWays,      ///< number of ways or associativity
        EvictionPolicy policy = cpu::cache::simpleEvictionPolicy<Cache>
    ):
        numberLines(numberLines), numberWays(numberWays),
        indexAddrBits(cgm::ct::math::log2(numberLines)),
        allAddrBits(Line::Data::addrBits + kTagSize + indexAddrBits),
        indexAddrMask(bits::make1s(indexAddrBits - 1)),
        hitCount(0), missCount(0), evictionPolicy(policy),
        lines(numberLines*numberWays)
    {
        static_assert((sizeof(typename Line::Tag::value_type)/8) <= kTagSize);
        static_assert((sizeof(typename Line::Flags::value_type)/8) <= kFlagsSize);
        static_assert(kDataSize % sizeof(Word) == 0, "data size word aligned");
    }

    size_t numWays() const { return numberWays; }
    size_t numLines() const { return numberLines; }

    /*struct Line
    {
        static const size_t sizeBits = kTagSize + kDataSize + kFlagsSize;

        using Tag = cache::Tag<uint64_t>;
        Tag tag{kTagSize,0};

        using Data = cache::Data<kDataSize,TWord>;
        Data data;

        using Flags = cache::Flags<uint64_t>;
        Flags flags{kFlagsSize,0};

        bool isValid() const {return flags.bitAt(kLineValidBit);}
        void setValid() {flags.setBit(kLineValidBit);}
        void clearValid() {flags.clearBit(kLineValidBit);}
        void invalidate() {clearValid();}

        static
        typename Tag::value_type getAddrTag(Addr addr) {
            return (addr >> (Data::addrBits + indexAddrBits)) & tagAddrMask;
        }

        static size_t getAddrByteId(Addr addr) { return addr & dataAddrMask; }
        static size_t getAddrWordId(Addr addr) { return getAddrByteId(addr)/sizeof(Word); }

        bool isAddrTagMatch(Addr addr) const {
            return getAddrTag(addr) == tag.val;
        }

    };*/

    //static const size_t indexAddrBits = cgm::ct::math::log2(kNumberLines);
    //static const size_t addrBits = Line::Data::addrBits + Line::Tag::sizeBits + indexAddrBits;

    //static const unsigned dataAddrMask = bits::make1s(Line::Data::addrBits - 1);
    //static const unsigned indexAddrMask = bits::make1s(indexAddrBits - 1);
    //static const unsigned tagAddrMask = bits::make1s(Line::Tag::sizeBits - 1);

    void invalidateAll() {
        for (auto& line : lines) {
            line.invalidate();
        }
    }

    Line& getLine(size_t setId, size_t wayId) {
        return lines.at(numberWays*setId + wayId);
    }

    const Line& getLine(size_t setId, size_t wayId) const {
        return lines.at(numberWays*setId + wayId);
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
        size_t setId{getLineIndexByAddr(addr)}, wayId{0}, wordId{Line::getAddrWordId(addr)};

        for (wayId = 0; wayId < numberWays; ++wayId) {
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
