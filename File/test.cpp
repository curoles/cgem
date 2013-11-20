#include <cassert>
#include <iostream>

#include "cgem/File.h"

using namespace std;

int main()
{
    File file;

    auto fname = "test.txt";

    if (File::is_exist(fname))
    {
        cout << "File " << fname << " exists, let's delete it\n";
        auto removed = File::remove (fname);
        assert (removed);
    }

    assert(not File::is_exist(fname));

    auto touched = File::touch(fname);

    assert(File::is_exist(fname));

    auto opened = file.open("test.txt", "r");

    assert(opened);

    assert(file.is_open());

    file.close();

    assert(not file.is_open());

    File::open (fname, "w", [](File& f)->bool{
        cout << "writing into file...\n";
        f.write("Hello world!\n");
        f.write("I am testing class File\n");
        return true;
    });

    File::open (fname, "r", [](File& f)->bool{
        cout << "reading file...\n";
        string line;
        while (f.read_line(line))
        {
            cout << line;
        }

        return true;
    });

    auto text = File::read(fname);

    cout << text << "\n";

    cout << "File: TEST PASSED\n";

    return 0;
}
