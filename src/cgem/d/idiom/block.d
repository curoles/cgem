module cgem.d.idiom.block;

// With delegate(s) one can mimic Ruby's block/yield.

template Block(R, T...)
{
    alias Block = R delegate(T);
}

version(unittest)
{
    import std.stdio;
    import std.conv;
}

unittest
{
    writeln("Testing module:" ~ __MODULE__);
}


unittest
{
    alias void delegate(int) FooBlock;

    void foo(FooBlock/*=void delegate(int)*/ yield)
    {
        for (int i = 0; i < 3; i++) {
            writeln("foo iteration #" ~ to!string(i));
            yield(i);
        }
    }

    int context = 0;

    foo(delegate void(int i){
        writeln("got " ~ to!string(i) ~ ", context=" ~ to!string(context));
        context++;
    });

    foo((i)/*=delegate void(int i)*/{
        writeln("got " ~ to!string(i) ~ ", context=" ~ to!string(context));
        context++;
    });

    int bar(int i, Block!(int,int,int) yield)
    {
        auto res = yield(i, i+context);
        return res;
    }

    bar(7, /*do*/ (i,j){
        writefln("got i=%d j=%d context=%d", i, j, context);
        return (j + i);
    });

}

unittest
{
    int context = 3;

    int bar(int i, Block!(int,int,int) yield)
    {
        auto res = yield(i, i+context);
        return res;
    }

    auto res = bar(7, /*do*/ (i,j){
        //writefln("got i=%d j=%d context=%d", i, j, context);
        assert(context == 3);
        assert(i == 7 && j == (7+context));
        return (j + i);
    });

    assert(res == (7 + 7 + 3));
}
