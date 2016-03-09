/**@file
 *  Authors:   Igor Lesik.
 *  Copyright: Igor Lesik 2009.
 *  License:   Distributed under the Boost Software License, Version 1.0.
 *            (See http://www.boost.org/LICENSE_1_0.txt)
 *
 *  Trie data structure.
 *
 *
 * This Trie immplementation is based on vectors:
 "abs", "add", "sub", "mov", "mov32"
 |a
 | b
 |  s
 |   :0
 | d
 |  d
 |   :1
 |s
 | u
 |  b
 |   :2
 |m
 | o
 |  v
 |   :3
 |   3
 |    2
 |     :4

 a[b[s]d[d]]s[u[b]]m[o[v[3[2]]]]
*/
#pragma once
#ifndef CGEM_TRIE_H_INCLUDED
#define CGEM_TRIE_H_INCLUDED

#include <assert.h>
#include <string.h>
#include <stdint.h>

#include "cgem/c/Vector/vector.h"

typedef Vector Trie;

void Trie_init(Trie* t);

void Trie_store(Trie* t, const char* str, size_t val);

int Trie_find(Trie* t, const char* str, size_t* val);

void Trie_destroy(Trie* t);

void Trie_print(Trie* t, size_t level);


#endif

