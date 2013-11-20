#include <cassert>
#include <iostream>

#include "cgem/StaticString.h"
#include "cgem/static_assert.h"

using namespace std;


constexpr StaticString test_string("Hi Mom!");

STATIC_ASSERT(test_string.size() == 7);
STATIC_ASSERT(test_string[6] == '!');

constexpr StaticString mom_str(test_string, 3);
STATIC_ASSERT(mom_str.size() == 4);
STATIC_ASSERT(mom_str[0] == 'M');
STATIC_ASSERT(mom_str[1] == 'o');
STATIC_ASSERT(mom_str[2] == 'm');

constexpr StaticString mom_str2(test_string, 3, 4);
STATIC_ASSERT(mom_str2 == "Mom!");

STATIC_ASSERT(StaticString::count_char("hello", 'l') == 2);

STATIC_ASSERT(StaticString::count_tokens("1 2 3") == 3);
STATIC_ASSERT(StaticString::count_tokens("one,two,three", ',') == 3);

constexpr StaticString test_strings1[] {{"one"}, {"two"}, {"three"}};

constexpr StaticString test_strings2[StaticString::count_tokens("1 2 3")] {{"1"}, {"2"}, {"3"}};


STATIC_ASSERT(sizeof(test_strings1)/sizeof(test_strings1[0]) == 3);


int main()
{
    cout << "Test string:" << test_string << "\n";
    cout << "Mom sub-string:" << mom_str << "\n";

    assert(string(mom_str) == "Mom!");

    cout << "StaticString: TEST PASSED" << "\n";

    return 0;
}
