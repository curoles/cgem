module cgem.d.io.file;

import std.stdio;
import cgem.d.idiom.block;

bool openFile(
    string file_name,
    in char[] stdioOpenmode,
    Block!(bool,std.stdio.File) yield
)
in
{
    assert(file_name.length > 0);
}
body
{
    std.stdio.File file = std.stdio.File(file_name, stdioOpenmode);
    scope (exit) file.close();

    if (!file.isOpen()) return false;

    scope (failure) return false;
    return yield(file);
}



version(unittest)
{
    import std.stdio;
    import std.conv;
    import std.file;
}

unittest
{
    writeln("Testing module:" ~ __MODULE__);

    string file_name = "dig_io_file_unittest.txt";
    std.file.write(file_name, "Testing dig.io.file\nend\n");

    bool res = openFile(file_name, "r+", /*do*/(file) {
        //writeln("Playing with file");
        string line1 = file.readln();
        //write(line1);
        string line2 = file.readln();
        assert(line2 == "end\n"); //write(line2);
        file.writeln("tested\n");
        return true;
    });

    assert (res == true);
}

