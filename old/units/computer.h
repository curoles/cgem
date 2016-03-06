#pragma once
//http://en.cppreference.com/w/cpp/language/user_literal
//
//type safe literals:
//http://scrupulousabstractions.tumblr.com/

namespace gem { namespace units {

namespace operators {

//http://en.cppreference.com/w/cpp/types/is_integral
//
/*template<class T>
T operator"" _KiB(T val) {
    static_assert(std::integral<T>::value, "bytes are of an integral type");
    return val * 1024;
}*/

inline constexpr unsigned long long operator"" _KiB(unsigned long long val) {
    return val * 1024ull;
}

// _KiB unit test
static_assert(3_KiB == 3 * 1024ul, "");

} // operators

}}

#include <cassert>

void test1()
{
    using namespace gem::units::operators;
    long unsigned int i7Kb = 7_KiB;
    assert(i7Kb == (7*1024));
}
