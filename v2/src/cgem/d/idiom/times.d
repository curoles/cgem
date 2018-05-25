module cgem.d.idiom.times;

import std.traits;
import cgem.d.idiom.block;

/** Ruby style N.times {|n| ...}.
 *
 * We use template constrains so that
 * T could be only some integral type.
 */
uint times(T)(in T number, Block!(void, uint) yield)
    if (isIntegral!(T) /*TODO || has(0,+1,less_than)*/)
{
    uint i = 0;

    if (number > i) {
    while (i < number) {
        yield(i);
        ++i;
    }}

    return i;
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

///
unittest
{
    auto counter = 0;
    int N = 10;
    N.times( (n) {
        //writeln(to!string(n));
        assert(n == counter);
        ++counter;
    });
    assert(counter == 10);

    10.times( (n) {
        ++counter;
    });
    assert(counter == 20);

}
