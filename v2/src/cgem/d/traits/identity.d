module dig.traits.identity;

/** Identity, a primitive notion of our perception of reality,
 *  determines the sameness of a thing changing over time.
 *
 *  Here we define identity as thing's name. TODO
 */


import std.traits;

version(unittest)
{
    import std.stdio;
}

unittest
{
    writeln("Testing module:" ~ __MODULE__);
}


string getIdentity(T...)()
{
    return std.traits.fullyQualifiedName!T;
}

string getIdentityByName(string name)()
{
    return std.traits.fullyQualifiedName!(mixin(name));
}

unittest
{
    int i=2;
    struct S { int i; void foo(){}}
    S s;
    writeln(getIdentity!i);
    writeln(getIdentity!S);
    //writeln(getIdentity!(S.i));
    //writeln(getIdentity!(s.foo));
}

//@name("test class")
unittest
{
    class C {
        int _i;
    }

    C c = new C();
    writeln(c.toString());
    writeln(getIdentity!C);
    writeln(getIdentity!c);
    //writeln(c._i.toString()); //Error: no property 'toString' for type 'int'
    static assert(is (typeof(c._i) == int));
    static assert(is (typeof(c) == class));
}

unittest
{
    struct S {
        int _i;
    }

    S s = S();
    //writeln(s.toString()); //Error: no property 'toString' for type 'S'
    //s.toHash(); //Error: no property 'toHash'
    writeln(getIdentity!S);
    writeln(getIdentity!s);
    writeln(getIdentityByName!("object"));

    static assert(is (typeof(s) == struct), "S is not structure");

}


unittest
{
    writeln(getIdentity!(mixin(__MODULE__)));
    writeln(getIdentityByName!(__MODULE__));
    //writeln(typeid(typeof(mixin(__MODULE__)))); Error: module identity has no type

    //static assert(is (typeof(s) == struct));

}
