/**@file
 * @brief     C ASCII string buffer with printf method.
 * @authors   Igor Lesik
 * @copyright Igor Lesik 2013.
 *
 * 
 */
#pragma once
#ifndef CGEM_STRING_BUFFER_H_INCLUDED
#define CGEM_STRING_BUFFER_H_INCLUDED

#include <string.h>

typedef struct gemStringBuf gemStringBuf;

struct gemStringBuf
{
    char*  m_storage_head;
    size_t m_allocated_size;
    char   m_owns_storage;

    char*  m_tail_ptr;

    void (*destruct)(struct gemStringBuf*);
    void (*reset)(struct gemStringBuf*);
    const char* (*c_str)(struct gemStringBuf*);
    size_t (*length)(struct gemStringBuf*);
    size_t (*available_size)(struct gemStringBuf*);
    int (*print)(struct gemStringBuf*, const char* frmt, ...);
    size_t (*realloc)(gemStringBuf*, size_t);
    const char* (*append)(gemStringBuf*, const char*);
};

#ifdef __cplusplus
extern "C" {
#endif

void gemStringBuf_construct(struct gemStringBuf* self, char* storage, size_t size);

#ifdef __cplusplus
}
#endif



#ifdef __cplusplus
namespace gem {

class StringBuf
{
    gemStringBuf buf;

public:
    StringBuf(char* storage=nullptr, size_t size=32) {
        gemStringBuf_construct(&buf, storage, size);
    }
   ~StringBuf() { buf.destruct(&buf); }

    void reset() { buf.reset(&buf); }

    size_t length() { return buf.length(&buf); }
    size_t available_size() { return buf.available_size(&buf); }

    size_t realloc(size_t size) { return buf.realloc(&buf, size); }

    const char* append(const char* s) { return buf.append(&buf, s); }

    int print(const char* frmt, ...);

    operator const char*() { return buf.m_storage_head; }
    const char* c_str() { return buf.m_storage_head; }

};

} // namespace gem
#endif

#endif
