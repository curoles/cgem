#include <stdio.h>

#include "cgem/C/trie.h"

int main()
{
    size_t i;
    size_t val;
    static char* dict[] = {"abs", "add", "sub", "mov", "mov32"};

    trie_t t;

    trie_init(&t);

    for (i = 0; i < (sizeof(dict)/sizeof(dict[0])); ++i) {
        trie_store(&t, dict[i], i);
    }
 
    printf("simple trie:\n");
    trie_print(&t, 0);
 
    for (i = 0; i < (sizeof(dict)/sizeof(dict[0])); ++i) {
        trie_find(&t, dict[i], &val);
        assert(val == i);
    }

    trie_destroy(&t);

    return 0;
}