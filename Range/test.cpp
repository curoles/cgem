#include <iostream>

#include "Range.h"

using namespace std;
using namespace gem;

int main()
{
    for (auto i : range(5, 16, 3))
    {
        cout << i << "\n";
    }

    for (auto f : range(5.1, 8.3, 0.6))
    {
        cout << f << "\n";
    }

    return 0;
}
