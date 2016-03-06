#include "gcc_pragma_macros.h"

CPP_VERSION
MESSAGE(Compiling...)

#include "swap_numbers.h"

int main()
{
    cgem::math::test_swap_nums();

    return 0;
}
