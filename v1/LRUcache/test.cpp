#include <cassert>
#include <iostream>

#include "cgem/LRUcache.h"

using namespace std;
using namespace gem;

typedef gem::LRUcache<int, int, 10> LRU;

int main()
{
    LRU cache;

    for (int key = 0; key < 15; ++key) {
        cache.put(key, key*10);
    }

    for (int key = 0; key < 15; ++key) {
        auto id = cache.find(key);
        if (cache.isValidId(id)) {
            std::cout << key << " data:" << cache.get(id) << "\n";
        }
        else {
            std::cout << key << " N/A\n";
        }
    }

    return 0;
}

