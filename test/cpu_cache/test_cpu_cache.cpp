/**@file
 * @brief Test CPU Cache model.
 *
 *
 */
#include "gtest/gtest.h"
#include "cgm/model/cpu_cache.h"



#if 0


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
