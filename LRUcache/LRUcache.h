#pragma once
 
#include <unordered_map>
#include <limits>
#include <cassert>

namespace gem {
 
template<class Key, class Data, std::size_t kSize>
class LRUcache
{
public:
    struct Entry
    {
        bool valid;
        Key  key;
        long int count;

        Data data;
    };


    // Entry Id is index in the 'entry' array
    typedef int EntryId;
 
    enum {kInvalidId = -1, kBonus = kSize};
  
    // Key to Entry mapping structure
    typedef std::unordered_map<Key, EntryId> HashTable;  
 
 
private:
    // Data storage as array indexed by entry id
    Entry entry[kSize];

    // Key to EntryId map
    HashTable mapping;

    unsigned long int nrHits;

public:
    LRUcache():nrHits(0) {
        invalidateAll();
    }

    static bool isValidId(EntryId id) /*const*/ { return id >=0 and id < kSize; }

    EntryId find(const Key& key) {
         auto it = mapping.find(key);
         if (it == mapping.end()) {
             return kInvalidId;
         }
         else {
             EntryId id = it->second;
             updateUsage(id);
             return id;
         }
    }
 
    Data& get(EntryId id) {
        assert(isValidId(id));
        return entry[id].data;
    }
 
    void invalidateAll() {
        for (EntryId id = 0; id < kSize; ++id) {
            entry[id].valid = false;
        }
        mapping.clear();
        nrHits = 0;
    }
 
    void updateUsage(EntryId id) {
        entry[id].count += kBonus;
        ++nrHits;
    }
 
    void put(const Key& key, const Data& data)
    {
        EntryId id = find(key);
        if (isValidId(id))
        {
            entry[id].data = data;
        }
        else
        {
            EntryId id = findLRUentry();
            assert(isValidId(id));
            Entry& e = entry[id];
            if (e.valid) {
                mapping.erase(e.key);
            }
            else {
                e.valid = true;
            }
 
            e.key = key;
            e.count = kBonus;
            e.data = data;
            mapping[key] = id;
            ++nrHits;
        }
    }

    EntryId findLRUentry()
    {
        EntryId freeEntryId{kInvalidId};
        EntryId lruEntryId{kInvalidId};
        long int minCount{std::numeric_limits<long int>::max()};

        //nrHits = nrHits % (kBonus*32);

        for (EntryId id = 0; id < kSize; ++id) {
            if (entry[id].valid) {
                entry[id].count -= nrHits;
                if (entry[id].count < minCount) {
                    minCount = entry[id].count;
                    lruEntryId = id;
                }
            }
            else {
                freeEntryId = id;
            }
        }

        nrHits = 0;

        return (isValidId(freeEntryId))? freeEntryId : lruEntryId;
    }
};


} // namespace gem
