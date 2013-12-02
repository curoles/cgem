#include <cassert>
#include <iostream>

#include "cgem/jrs.h"

using namespace std;
using namespace gem;

int main()
{
    const char fn[] = "test.jrs";

    if (File::is_exist(fn)) File::remove(fn);

    assert(not File::is_exist(fn));

    jrs::Journal jrnl;

    jrnl.open_new(fn);

    for (int tick = 0; tick < 200; ++tick)
    {
        jrnl.add_record_val(tick, tick);
    }

    jrnl.close();

    jrnl.open_append(fn);

    for (int tick = 0; tick < 200; ++tick)
    {
        jrnl.add_record_val(tick, tick);
    }

    jrnl.close();

    jrs::convert_to_text(fn, "test.jrs.txt");

    cout << "JRS: TEST PASSED\n";

    return 0;
}
