#include <stdio.h>

#define CGEM_VECTOR_UNIT_TEST
#include "cgem/c/Vector/vector.h"

int main()
{
    vector_unit_test();
    matrix_unit_test();

    return 0;
}
