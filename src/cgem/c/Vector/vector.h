/**@file
 * @authors   Igor Lesik
 * @copyright Igor Lesik 2009.
 * License:   Distributed under the Boost Software License, Version 1.0.
 *            (See http://www.boost.org/LICENSE_1_0.txt)
 *
 *  Dynamic array/vector and 2D matrix.
 *  All code is inlined.
 *
 *  See examples: vector_unit_test() and matrix_unit_test() in this file.
 */
#pragma once
#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

/** Resizable/dynamic vector in C.
 *
 *  Usage:
 *    vector_t v;
 *    vector_init(&v, sizeof(uint32));
 *    vector_push_back(uint32_t, v, 7);
 *    for ( i = 0; i < v.size; ++i) { ... vector_at(uint32_t, v, i) ... }
 *    vector_destroy(&v);
 *
 *   
 */
typedef struct vector_t
{
    size_t    el_sz;   ///< Size of an element in bytes.
    size_t    space;   ///< Number of allocated elements.
    size_t    size;    ///< Length/size of vector, i.e. # of elements.
    void*     element; ///< Allocated memory.
    size_t    reserved; 
} vector_t;

/// Number of elements to add on resize.
static const size_t vector_chunk_size = 10;

/// Make vector bigger, allocate more elements.
static inline void vector_resize(vector_t* v)
{
    assert(v != NULL);

    // resize when all space is used; assuming we use ONLY vector_push_back
    // to add new element to array
    if (v->size == v->space)
    {
        v->space += vector_chunk_size;
        void* new_space = realloc(v->element, v->el_sz * v->space);
        assert(new_space != NULL);
        v->element = new_space;
    }
}

/// Initialize vector structure.
static inline void vector_init(vector_t* v, size_t element_size)
{
    assert(v != NULL);
    assert(element_size > 0);

    v->el_sz = element_size;
    v->space = v->size = 0;
    v->element = NULL;

    vector_resize(v);
}

/// Free allocated memory.
static inline void vector_destroy(vector_t* v)
{
    if (v != NULL && v->element != NULL) {
        free(v->element);
        v->element = NULL;
    }
}

/// Access n-th element of the vector.
#define vector_at(T,v,n) (((T *)v.element)[n])

/// Insert new element in to vector.
#define vector_push_back(T,v,new_element)              \
{                                                      \
    assert(v.element != NULL);                         \
    assert(sizeof(T) == v.el_sz);                      \
                                                       \
    vector_resize(&v);                                 \
                                                       \
    ((T *)v.element)[v.size++] = new_element;          \
}

#ifdef VECTOR_UNIT_TEST
static inline void vector_unit_test()
{
    size_t i = 0;
    vector_t v;
    uint32_t** alias;

    vector_init(&v, sizeof(uint32_t));
    alias = (uint32_t**) &(v.element);

    for (i = 0; i < 22; ++i){
        vector_push_back(uint32_t, v, i);
    }

    for (i = 0; i < v.size; ++i){
        printf("%3u %3u\n", vector_at(uint32_t, v, i), (*alias)[i]);
        assert(i == vector_at(uint32_t, v, i));
    }

    vector_destroy(&v);
}
#endif

///////////////////////////// 2D VECTOR (MATRIX) //////////////////////////////

static inline void matrix_add_row(vector_t* mx)
{
    assert(mx != NULL && mx->element != NULL);

    size_t i = mx->size;
    vector_t v = {0,0,0,0};

    vector_push_back(vector_t,(*mx),v);
    vector_init(&vector_at(vector_t,(*mx),i), mx->reserved);
}

static inline void matrix_init(vector_t* mx, size_t el_sz, size_t n)
{
    size_t i;
    vector_init(mx, sizeof(vector_t));
    mx->reserved = el_sz;

    for (i = 0; i < n; ++i) {
        matrix_add_row(mx);
    }
}

static inline void matrix_destroy(vector_t* v)
{
    size_t i;
    if (v == NULL || v->element == NULL) return;

    for ( i = 0; i < v->size; ++i)
        vector_destroy(&vector_at(vector_t,(*v),i));

    vector_destroy(v);
}

#define matrix_at(T,v,n,m) vector_at(T,vector_at(vector_t,v,n),m)

#define matrix_push_back(T,v,n,new_element) vector_push_back(T,vector_at(vector_t,v,n),new_element)

#define matrix_row_size(mx,j) (vector_at(vector_t,mx,j).size)

#ifdef VECTOR_UNIT_TEST
static inline void matrix_unit_test()
{
    size_t i, j;

    vector_t mx;

    matrix_init(&mx, sizeof(uint32_t), 31);

    for ( j = 0; j < 31; ++j) {
        for ( i = 0; i < (j+1); ++i) { matrix_push_back(uint32_t, mx, j, i); }
    }

    matrix_add_row(&mx);
    matrix_push_back(uint32_t, mx, mx.size - 1, 0);

    for ( j = 0; j < mx.size; ++j) {
        for ( i = 0; i < matrix_row_size(mx,j); ++i) {
            printf("%3u ", matrix_at(uint32_t, mx, j, i));
            assert(i == matrix_at(uint32_t, mx, j, i));
        }
        printf("\n");
    }

    matrix_destroy(&mx);
}
#endif

#endif

