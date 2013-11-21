/**@file
 *  Authors:   Igor Lesik.
 *  Copyright: Igor Lesik 2009.
 *  License:   Distributed under the Boost Software License, Version 1.0.
 *            (See http://www.boost.org/LICENSE_1_0.txt)
 *
 *  Trie data structure.
 *
 */
#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED

#include <assert.h>
#include <string.h>
#include <stdint.h>

#include "vector.h"

typedef vector_t trie_t;

void trie_init(trie_t* t)
{
    vector_init(t, sizeof(vector_t));
    t->reserved = 0;
}

void trie_store(trie_t* t, const char* str, size_t val)
{
    size_t i, found = 0;

    for (i = 0; i < t->size; ++i) {
        if (vector_at(vector_t, (*t), i).reserved == str[0]) {
            found = 1;
            break;
        }
    }

    if (!found) {
        i = t->size;
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
    size_t i;

    for (i = 0; i < t->size; ++i)
    {
        if (vector_at(vector_t, (*t), i).element != NULL)
        {
            trie_destroy(&vector_at(vector_t, (*t), i));
            vector_destroy(&vector_at(vector_t, (*t), i));
        }
    }
}

#ifdef TRIE_UNIT_TEST

void trie_print(trie_t* t, size_t level)
{
    size_t i;
    char c;

    for (i = 0; i < t->size; ++i)
    {
        c = (char)vector_at(vector_t, (*t), i).reserved;

        if (c == '\0') { printf("%*s = %u\n", level, "", vector_at(vector_t, (*t), i).size); }
        else { printf("%*s%c\n", level, "", c); }

        if (vector_at(vector_t, (*t), i).element != NULL)
        {
            trie_print(&vector_at(vector_t, (*t), i), level + 1);
        }
    }
}

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
 
    trie_print(&t, 0);
 
    for (i = 0; i < (sizeof(dict)/sizeof(dict[0])); ++i) {
        trie_find(&t, dict[i], &val);
        assert(val == i);
    }

    trie_destroy(&t);
}
#endif // TRIE_UNIT_TEST

#endif

