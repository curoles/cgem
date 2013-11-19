#include <cassert>
#include <iostream>

#include "cgem/Lambda.h"

int main ()
{
    std::cout << "Lambda: start test\n";

    lambda(int)(int x, int y) { return (x > y)? x : y; }} max;
    
    assert(max(1, 2) == 2);

    lambda(int)(int x) {
       return (x <= 1)? 1 : (x * (*this)(x-1));
    }} factorial;

    assert(factorial(5) == 120);

    std::cout << "Lambda: TEST PASSED\n";

    return 0;
}

