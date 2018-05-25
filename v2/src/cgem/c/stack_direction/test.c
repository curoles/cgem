#include "cgem/c/stack_direction/stack_direction.h"

#include <stdio.h>
#include <assert.h>

int main()
{
    int direction = cgem__find_stack_direction();

    printf("Stack grows %s\n", (direction < 0)? "down":"up");

// GCC
#ifdef __i386__
    assert(direction < 0); // stack grows down on x86
#endif

    return 0;
}
