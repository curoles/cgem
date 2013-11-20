/**@file
 * @brief     C ASCII string buffer with printf-like method.
 * @authors   Igor Lesik
 * @copyright Igor Lesik 2013.
 *
 * 
 */
#include "StringBuf.h"

#include <stdarg.h>
#include <stdlib.h>

static
const char* gemStringBuf_c_str(struct gemStringBuf* self)
{
    return self->m_storage_head;
}

static
void gemStringBuf_reset(struct gemStringBuf* self)
{
    self->m_tail_ptr = self->m_storage_head;
}

static
size_t gemStringBuf_length(struct gemStringBuf* self)
{
    return (self->m_tail_ptr - self->m_storage_head);
}

static
size_t gemStringBuf_available_size(struct gemStringBuf* self)
{
    return (self->m_allocated_size - gemStringBuf_length(self));
}


/**
 * @return negative number if error
 */
static
int gemStringBuf_print(struct gemStringBuf* self, const char* frmt, ...)
{
    va_list ap;
    va_start(ap, frmt);
    int written = vsnprintf(self->m_tail_ptr, self->available_size(self), frmt, ap);
    va_end(ap);

    self->m_tail_ptr += written;

    return written;
}

static
void gemStringBuf_destruct(struct gemStringBuf* self)
{
    if (self->m_owns_storage)
    {
        free(self->m_storage_head);
        self->m_storage_head = NULL;
    }
}

static
size_t gemStringBuf_realloc(struct gemStringBuf* self, size_t new_size)
{
    if (self->m_owns_storage)
    {
        size_t old_length = self->length(self);

        self->m_storage_head = (char*) realloc(self->m_storage_head, new_size);
        if (self->m_storage_head != NULL)
        {
            self->m_allocated_size = new_size;
            self->m_tail_ptr = self->m_storage_head + old_length; 
        }
        else
        {
            return 0;
        }
    }

    return self->m_allocated_size;
}

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

static
const char* gemStringBuf_append(gemStringBuf* self, const char* str)
{
    if (self->available_size(self) < strlen(str))
    {
        if (self->m_owns_storage)
        {
            size_t increase = strlen(str) - self->available_size(self);
            size_t new_size = self->m_allocated_size + increase;
            self->realloc(self, new_size);
        }
        else
        {
            return NULL;
        }
    }

    strncpy(self->m_tail_ptr, str, self->available_size(self));
    self->m_tail_ptr += min(strlen(str), self->available_size(self));

    return self->m_storage_head;
}

void gemStringBuf_construct(struct gemStringBuf* self, char* storage, size_t size)
{
    if (storage)
    {
        self->m_owns_storage = 0;
        self->m_storage_head = storage;
        self->m_allocated_size = size;
    }
    else
    {
        self->m_owns_storage = 1;
        self->m_allocated_size = (size > 0)? size : 64;
        self->m_storage_head = (char*) calloc(self->m_allocated_size, 1);
    }

    self->destruct = gemStringBuf_destruct;
    self->realloc = gemStringBuf_realloc;
    self->reset = gemStringBuf_reset;
    self->c_str = gemStringBuf_c_str;
    self->length = gemStringBuf_length;
    self->available_size = gemStringBuf_available_size;
    self->print = gemStringBuf_print;
    self->append = gemStringBuf_append;

    self->reset(self);
}


