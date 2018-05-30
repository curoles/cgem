/**@file
 * @brief Test CPU Cache model.
 *
 *
 */
#include "gtest/gtest.h"
#include "cgm/model/cpu_cache.h"

constexpr size_t kNumberLines {16};
constexpr size_t kNumberWays   {2};
constexpr size_t kTagSize      {2};
constexpr size_t kDataSize    {64};
constexpr size_t kFlagsSize    {1};

using Cache =
    cgm::model::cpu::Cache<
        uint64_t,     // 64 bit address
        uint32_t,     // 32 bit data word
        kTagSize,
        kDataSize,
        kFlagsSize
    >;

TEST(CpuCache, Creation) {
    Cache cache{kNumberLines, kNumberWays};

    cache.invalidateAll();

    ASSERT_EQ (cache.numLines(), kNumberLines);
    ASSERT_EQ (cache.numWays(), kNumberWays);

    ASSERT_EQ (cache.getLine(0,0).tag.size, kTagSize);
    ASSERT_EQ (cache.getLine(1,1).data.size(), kDataSize/32);
    ASSERT_EQ (Cache::Line::Data::sizeBits_, kDataSize);
    ASSERT_EQ (Cache::Line::Data::sizeBytes, kDataSize/8);
    ASSERT_EQ (Cache::Line::Data::sizeWords, kDataSize/(8*sizeof(Cache::Word)));
    ASSERT_EQ (Cache::Line::Data::addrBits, 3/*64/8=8=2^3*/);
    ASSERT_EQ (cache.getLine(2,0).flags.size, kFlagsSize);

    //std::cout << std::dec << "total # of addr bits:" << cache.addrBits << "\n";
    //std::cout << std::dec << "# of data addr bits:" << Cache::Line::Data::addrBits << "\n";
    //std::cout << std::dec << "# of index addr bits:" << Cache::indexAddrBits << "\n";

    ASSERT_EQ (cache.indexAddrBits, 4/*log2(16)*/);
    ASSERT_EQ (cache.allAddrBits, 2+4+3);
}

TEST(CpuCache, Bounds) {
    Cache cache{kNumberLines, kNumberWays};

    bool hadException {false};
    try {
        cache.getLine(kNumberLines,0);
    } catch(std::out_of_range& e) {
        hadException = true;
    }
    ASSERT_TRUE (hadException);
}

TEST(CpuCache, InsertLine) {
    Cache cache{kNumberLines, kNumberWays};

    cache.invalidateAll();

}

#if 0


#include <cassert>
#include <stdexcept>
#include <iostream>

int test()
{
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
