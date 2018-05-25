#pragma once

#include <cstdio>
#include <ostream>
#include <functional>

namespace gem {

// You may need to pass additional flags to the linker to make the
// function names available to the program.
// For example, on systems using GNU ld, you must pass (-rdynamic.) 
//
class Backtrace
{
public:
    static
    void get(std::function<void (int, const char*)> printer);

    static
    void print(FILE* stream=stdout);

    static
    void print(std::ostream& stream);

};

}
