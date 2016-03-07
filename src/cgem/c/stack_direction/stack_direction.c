/**@file
 * @brief  Find what direction stack grows.
 * @author Igor Lesik
 */
#include "cgem/c/stack_direction/stack_direction.h"

/**
 * @return 1 if up, -1 if down
 */
int cgem__find_stack_direction()
{
    auto char dummy;                   /* variable on stack */
    static char* addr_of_dummy = 0;    /* var to save addr of dummy 1st time */
 
    if (addr_of_dummy == 0)            /* first entry into the function */
    {
        addr_of_dummy = &dummy;        /* save addr of variable on stack */
        return cgem__find_stack_direction(); /* call itself to compare addresses */
    }
    else                               /* second entry into the function */
        return (&dummy > addr_of_dummy)?  1 /* stack grew upward   */
                                       : -1 /* stack grew downward */;
}

