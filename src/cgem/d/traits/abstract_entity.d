module dig.traits.abstract_entity;
//@igor TODO file bug, can't use keyword as package/module name, for example "abstract".

/** An abstract entity is an individual thing that is eternal
 *  and unchangeable. Thus type that can't be instantiated
 *  is an abstract entity. 
 */
bool isAbstractEntityType(T)()
{
    return
        __traits(isAbstractClass, T)       // abstract class cannot be instantiated directly
        /* isInterface=isAbstractClass */  // cannot create instance of interface
    ;
}

/** A concrete entity is an individual thing that comes into and out
 *  of existence.
 */
bool isConcreteEntityType(T)()
{
    return !isAbstractEntityType!T;
}


version(unittest)
{
    import std.stdio;
}

unittest
{
    writeln("Testing module:" ~ __MODULE__);
}

/// isAbstractEntityType usage
unittest
{
    static assert( !isAbstractEntityType!int );

    interface Interface { int fun(); }
    abstract class AbstractClass : Interface { void bar(); }
    class ConcreteClass : AbstractClass {
        override void bar(){}
        int fun() {return 0;}
    }

    static assert( isAbstractEntityType!Interface );
    static assert( isAbstractEntityType!AbstractClass );
    static assert(!isAbstractEntityType!ConcreteClass );
    static assert( isConcreteEntityType!ConcreteClass );
}

unittest
{
    class VeryConcrete
    {
        int i;

        this(this ClassT)()
        in {
            static assert(isConcreteEntityType!ClassT);
        }
        body{
            writeln("constructing class type:" ~ std.traits.fullyQualifiedName!ClassT);
        }
    }
 
    static assert( isConcreteEntityType!VeryConcrete );
    VeryConcrete instance = new VeryConcrete;
}
