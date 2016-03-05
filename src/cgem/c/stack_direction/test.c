#include "cgem/c/stack_direction.h"

#include <stdio.h>

int main()
{
    printf("Stack grows %s\n", (cgem__find_stack_direction() < 0)? "down":"up");

    return 0;
}
