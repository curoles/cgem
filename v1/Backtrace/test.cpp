#include <cstdio>
#include <iostream>

#include "cgem/Backtrace.h"

int main()
{
    std::cout << "backtrace example 1:\n\n";

    gem::Backtrace::get([](int i, const char* str){
        printf("bt[%d] %s\n", i, str);
    });

    std::cout << "backtrace example 2:\n\n";

    gem::Backtrace::print();

    std::cout << "backtrace example 3:\n\n";

    gem::Backtrace::print(std::cout);

    return 0;
}
