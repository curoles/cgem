#include <iostream>
#include <chrono>

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

#if 0
    int temp(0);

    auto start1 = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 1000000; ++i)
    {
        temp += i;
    }
    auto end1 = std::chrono::high_resolution_clock::now();

    auto start2 = std::chrono::high_resolution_clock::now();
    for(auto i : range(0, 1000000))
    {
        temp += i;
    }
    auto end2 = std::chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed_seconds1 = end1 - start1;
    chrono::duration<double> elapsed_seconds2 = end2 - start2;

    cout << "Timing C array = " << elapsed_seconds1.count() <<
            " vs. gem::Range = " << elapsed_seconds2.count() << "\n";

    cout << temp;
#endif

    return 0;
}
