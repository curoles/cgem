#include <stdio.h>

#include "cgem/c/Trie/trie.h"

int main()
{
    size_t i;
    size_t val;
    static char* dict[] = {"abs", "add", "sub", "mov", "mov32"};

    Trie t;

    Trie_init(&t);

    for (i = 0; i < (sizeof(dict)/sizeof(dict[0])); ++i) {
        Trie_store(&t, dict[i], i);
    }
 
    printf("simple trie:\n");
    Trie_print(&t, 0);
 
    for (i = 0; i < (sizeof(dict)/sizeof(dict[0])); ++i) {
        Trie_find(&t, dict[i], &val);
        assert(val == i);
    }

    Trie_destroy(&t);

    return 0;
}
