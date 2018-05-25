#include "cgem/StringBuf.h"

#include <assert.h>
#include <stdio.h>

int main()
{
    char storage[128];

    gemStringBuf buf;
    gemStringBuf_construct(&buf, storage, sizeof(storage));

    assert(buf.length(&buf) == 0);
    assert(buf.available_size(&buf) == sizeof(storage));

    buf.print(&buf, "hello %d", 123);

    assert(buf.length(&buf) == strlen("hello 123"));
    assert(buf.available_size(&buf) == (sizeof(storage) - strlen("hello 123")));

    printf("%s\n", buf.c_str(&buf));

    buf.print(&buf, "%c %s", '!', "test");

    printf("%s\n", buf.c_str(&buf));

    buf.reset(&buf);
    buf.print(&buf, "%c", '1');
    assert(buf.length(&buf) == strlen(buf.c_str(&buf)));

    assert(buf.c_str(&buf)[1] == '\0');

    buf.destruct(&buf);

    gemStringBuf_construct(&buf, NULL, 16);

    buf.print(&buf, "hello %d", 123);
    buf.realloc(&buf, 32);
    buf.append(&buf, "4 extra");
    printf("%s\n", buf.c_str(&buf));

    buf.destruct(&buf);

    return 0;
}

