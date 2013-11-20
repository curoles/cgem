/**@file
 * @brief     C ASCII string buffer with printf method.
 * @authors   Igor Lesik
 * @copyright Igor Lesik 2013.
 *
 * 
 */
#include "StringBuf.h"

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


/**
 * @return negative number if error
 */
int
gem::StringBuf::
print(const char* frmt, ...)
{
    va_list ap;
    va_start(ap, frmt);
    int written = vsnprintf(buf.m_tail_ptr, available_size(), frmt, ap);
    va_end(ap);

    if (written >= available_size())
    {
        realloc(buf.m_allocated_size + (written - available_size()) + 1);
        assert(available_size() > written);
        va_start(ap, frmt);
        written = vsnprintf(buf.m_tail_ptr, available_size(), frmt, ap);
        va_end(ap);
    }

    buf.m_tail_ptr += written;

    return written;
}


