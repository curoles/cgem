#include "cgem/C/stack_direction.h"

#include "stdio.h"

int main()
{
    printf("Stack grows %s\n", (find_stack_direction() < 0)? "down":"up");

    return 0;
}
