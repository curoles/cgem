#include "gcc_pragma_macros.h"

MESSAGE("Compiling...")

#include "swap_numbers.h"

int main()
{
    cgem_math__test_swap_nums_uint32();
    cgem_math__test_swap_halfs_uint32();

    return 0;
}
