#include "StringBuf.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int main()
{
    gem::StringBuf buf(NULL, 2);

    assert(buf.length() == 0);
    assert(buf.available_size() == 2);

    buf.append("hello!");
    assert(buf.length() == 6);
    assert(buf.available_size() == 0);

    buf.print(" %s", "extra");

    buf.append("! ");

    buf.print(" %c", '.');

    printf("%s\n", buf.c_str());

    gem::StringBuf xml;
    xml.print(
R"XML(<tag0>
  <str>%s</str>
  <num>%d</num>
</tag0>)XML", "hello", 123);
    printf("%s\n", xml.c_str());

    return 0;
}

