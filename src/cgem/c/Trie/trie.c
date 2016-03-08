/**@file
 *  Authors:   Igor Lesik.
 *  Copyright: Igor Lesik 2009.
 *  License:   Distributed under the Boost Software License, Version 1.0.
 *            (See http://www.boost.org/LICENSE_1_0.txt)
 *
 *  Trie data structure.
 *
 */
#include "trie.h"

#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

void trie_init(trie_t* t)
{
    vector_init(t, sizeof(vector_t));
    t->reserved = 0;
}

void trie_store(trie_t* t, const char* str, size_t val)
{
    size_t i, found = 0;

    // scan top row for first character
    for (i = 0; i < t->size; ++i) {
        if (vector_at(vector_t, (*t), i).reserved == (size_t)str[0]) {
            found = 1;
            break;
        }
    }

    if (!found) {
        i = t->size; // end of the vector, i.e. back+1
        vector_t v = {0,0,0,NULL,str[0]};
        vector_push_back(vector_t, (*t), v);
        if (str[0] != '\0') {
            vector_init(&vector_at(vector_t,(*t),i), sizeof(vector_t));
        }
    }

    if (str[0] == '\0') {
        vector_at(vector_t,(*t),i).size = val;
    }
    else {
        trie_store(&vector_at(vector_t,(*t),i), str + 1, val);
    }
}

int trie_find(trie_t* t, const char* str, size_t* val)
{
    uint32_t v; char c; size_t i;

    for (i = 0; i < t->size; ++i)
    {
        v = vector_at(vector_t, (*t), i).size; // node value stored in "size" field.
        c = (char)vector_at(vector_t, (*t), i).reserved;

        if (c == str[0])
        {
            if (vector_at(vector_t, (*t), i).element == NULL) {
                *val = v;
                return 1;
            }
            else {
                return trie_find(&vector_at(vector_t, (*t), i), str + 1, val);
            }
        }
    }

    return 0;
}

void trie_destroy(trie_t* t)
{
    size_t i, size = t->size;
    assert(t);

    for (i = 0; i < size; ++i)
    {
        if (vector_at(vector_t, (*t), i).element != NULL)
        {
            trie_destroy(&vector_at(vector_t, (*t), i));
        }
    }
    vector_destroy(t);
}

void trie_print(trie_t* t, size_t level)
{
    size_t i;
    char c;

    for (i = 0; i < t->size; ++i)
    {
        c = (char)vector_at(vector_t, (*t), i).reserved;

        if (c == '\0')
        {
            printf("%*s:%zu\n", (int)level, "", vector_at(vector_t, (*t), i).size);
        }
        else
        {
            printf("%*s%c\n", (int)level, "", c);
        }

        if (vector_at(vector_t, (*t), i).element != NULL)
        {
            trie_print(&vector_at(vector_t, (*t), i), level + 1);
        }
    }
}

#if 0
void trie_unit_test()
{
    size_t i;
    size_t val;
    static char* dict[] = {"abs", "add", "sub", "mov", "mov32"};

    trie_t t;

    trie_init(&t);

    for (i = 0; i < (sizeof(dict)/sizeof(dict[0])); ++i) {
        trie_store(&t, dict[i], i);
    }
 
    //printf("simple trie:\n");
    //trie_print(&t, 0);
 
    for (i = 0; i < (sizeof(dict)/sizeof(dict[0])); ++i) {
        trie_find(&t, dict[i], &val);
        assert(val == i);
    }

    trie_destroy(&t);
}
#endif


