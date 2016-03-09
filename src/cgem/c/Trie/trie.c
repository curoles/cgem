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

void Trie_init(Trie* t)
{
    Vector_init(t, sizeof(Vector));
    t->reserved = 0;
}

void Trie_store(Trie* t, const char* str, size_t val)
{
    size_t i, found = 0;

    // scan top row for first character
    for (i = 0; i < t->size; ++i) {
        if (Vector_at((*t), i, Vector).reserved == (size_t)str[0]) {
            found = 1;
            break;
        }
    }

    if (!found) {
        i = t->size; // end of the vector, i.e. back+1
        Vector v = {0,0,0,NULL,str[0]};
        Vector_push_back((*t), Vector, v);
        if (str[0] != '\0') {
            Vector_init(&Vector_at((*t),i,Vector), sizeof(Vector));
        }
    }

    if (str[0] == '\0') {
        Vector_at((*t),i,Vector).size = val;
    }
    else {
        Trie_store(&Vector_at((*t),i,Vector), str + 1, val);
    }
}

int Trie_find(Trie* t, const char* str, size_t* val)
{
    uint32_t v; char c; size_t i;

    for (i = 0; i < t->size; ++i)
    {
        v = Vector_at((*t), i, Vector).size; // node value stored in "size" field.
        c = (char)Vector_at((*t), i, Vector).reserved;

        if (c == str[0])
        {
            if (Vector_at((*t), i, Vector).element == NULL) {
                *val = v;
                return 1;
            }
            else {
                return Trie_find(&Vector_at((*t), i, Vector), str + 1, val);
            }
        }
    }

    return 0;
}

void Trie_destroy(Trie* t)
{
    size_t i, size = t->size;
    assert(t);

    for (i = 0; i < size; ++i)
    {
        if (Vector_at((*t), i, Vector).element != NULL)
        {
            Trie_destroy(&Vector_at((*t), i, Vector));
        }
    }
    Vector_destroy(t);
}

void Trie_print(Trie* t, size_t level)
{
    size_t i;
    char c;

    for (i = 0; i < t->size; ++i)
    {
        c = (char)Vector_at((*t), i, Vector).reserved;

        if (c == '\0')
        {
            printf("%*s:%zu\n", (int)level, "", Vector_at((*t), i, Vector).size);
        }
        else
        {
            printf("%*s%c\n", (int)level, "", c);
        }

        if (Vector_at((*t), i, Vector).element != NULL)
        {
            Trie_print(&Vector_at((*t), i, Vector), level + 1);
        }
    }
}

#if 0
void trie_unit_test()
{
    size_t i;
    size_t val;
    static char* dict[] = {"abs", "add", "sub", "mov", "mov32"};

    Trie t;

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


