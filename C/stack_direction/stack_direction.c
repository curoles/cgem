/**@file
 * @brief Find what direction stack grows
 */
#include "stack_direction.h"

/**
 * @return 1 if up, -1 if down
 */
int find_stack_direction()
{
    auto char dummy;
    static char* addr_of_dummy = 0;
 
    if (addr_of_dummy == 0) /* first entry */
    {
        addr_of_dummy = &dummy;
        return find_stack_direction(); /* recurse once */
    }
    else /* second entry */
        return (&dummy > addr_of_dummy)?  1 /* stack grew upward   */
                                       : -1 /* stack grew downward */;
}

