/**@file
 * @authors   Igor Lesik
 * @copyright Igor Lesik 2009-2016.
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
 *    Vector v;
 *    Vector_init(&v, sizeof(uint32));
 *    Vector_push_back(v, uint32_t, 7);
 *    for (i = 0; i < v.size; ++i) { ... Vector_at(v, i, uint32_t) ... }
 *    Vector_destroy(&v);
 *
 *   
 */
typedef struct Vector
{
    size_t    el_sz;   ///< Size of an element in bytes.
    size_t    space;   ///< Number of allocated elements.
    size_t    size;    ///< Length/size of vector, i.e. # of elements.
    void*     element; ///< Allocated memory.
    size_t    reserved; 
} Vector;

/// Number of elements to add on resize.
static const size_t vector_chunk_size = 10;

/// Make vector bigger, allocate more elements.
static inline void Vector_resize(Vector* v)
{
    assert(v != NULL);

    // resize when all space is used; assuming we use ONLY Vector_push_back
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
static inline void Vector_init(Vector* v, size_t element_size)
{
    assert(v != NULL);
    assert(element_size > 0);

    v->el_sz = element_size;
    v->space = v->size = 0;
    v->element = NULL;

    Vector_resize(v);
}

/// Free allocated memory.
static inline void Vector_destroy(Vector* v)
{
    if (v != NULL && v->element != NULL) {
        free(v->element);
        v->element = NULL;
    }
}

/// Access n-th element of the vector.
#define Vector_at(v,n,T) (((T *)v.element)[n])

/// Insert new element in to vector.
#define Vector_push_back(v,T,new_element)              \
{                                                      \
    assert(v.element != NULL);                         \
    assert(sizeof(T) == v.el_sz);                      \
                                                       \
    Vector_resize(&v);                                 \
                                                       \
    ((T *)v.element)[v.size++] = new_element;          \
}

#ifdef CGEM_VECTOR_UNIT_TEST
static inline void vector_unit_test()
{
    size_t i = 0;
    Vector v;
    uint32_t** alias;

    Vector_init(&v, sizeof(uint32_t));
    alias = (uint32_t**) &(v.element);

    for (i = 0; i < 22; ++i){
        Vector_push_back(v, uint32_t, i);
    }

    for (i = 0; i < v.size; ++i){
        printf("%3u %3u\n", Vector_at(v, i, uint32_t), (*alias)[i]);
        assert(i == Vector_at(v, i, uint32_t));
    }

    Vector_destroy(&v);
}
#endif

///////////////////////////// 2D VECTOR (MATRIX) //////////////////////////////

static inline void Matrix_add_row(Vector* mx)
{
    assert(mx != NULL && mx->element != NULL);

    size_t i = mx->size;
    Vector v = {0,0,0,0};

    Vector_push_back((*mx),Vector,v);
    Vector_init(&Vector_at((*mx),i,Vector), mx->reserved);
}

static inline void Matrix_init(Vector* mx, size_t el_sz, size_t n)
{
    size_t i;
    Vector_init(mx, sizeof(Vector));
    mx->reserved = el_sz;

    for (i = 0; i < n; ++i) {
        Matrix_add_row(mx);
    }
}

static inline void Matrix_destroy(Vector* v)
{
    size_t i;
    if (v == NULL || v->element == NULL) return;

    for (i = 0; i < v->size; ++i) {
        Vector_destroy(&Vector_at((*v),i,Vector));
    }

    Vector_destroy(v);
}

#define Matrix_at(v,n,m,T) Vector_at(Vector_at(v,n,Vector),m,T)

#define Matrix_push_back(v,n,T,new_element) Vector_push_back(Vector_at(v,n,Vector),T,new_element)

#define Matrix_row_size(mx,row) (Vector_at(mx,row,Vector).size)

#ifdef CGEM_VECTOR_UNIT_TEST
static inline void matrix_unit_test()
{
    size_t i, j;

    Vector mx;

    Matrix_init(&mx, sizeof(uint32_t), 31);

    for ( j = 0; j < 31; ++j) {
        for ( i = 0; i < (j+1); ++i) { Matrix_push_back(mx, j, uint32_t, i); }
    }

    Matrix_add_row(&mx);
    Matrix_push_back(mx, mx.size - 1, uint32_t, 0);

    for ( j = 0; j < mx.size; ++j) {
        for ( i = 0; i < Matrix_row_size(mx,j); ++i) {
            printf("%3u ", Matrix_at(mx, j, i, uint32_t));
            assert(i == Matrix_at(mx, j, i, uint32_t));
        }
        printf("\n");
    }

    Matrix_destroy(&mx);
}
#endif

#endif

