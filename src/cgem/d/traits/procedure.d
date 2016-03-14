/**
 * Procedures.
 *
 * Authors:   Igor Lesik 2015
 * Copyright: (C) Igor Lesik
 *
 * License:
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See_Also:
 * Stepanov. Elements of Programming.
 *
 * See_Also:
 * $(LINK https://github.com/D-Programming-Language/phobos/blob/master/std/traits.d)
 *
 * See_Also:
 * $(LINK http://www.drdobbs.com/architecture-and-design/pure-functions/228700129)
 * $(LINK http://klickverbot.at/blog/2012/05/purity-in-d)
 */
module dig.traits.procedure;

import std.traits;
import std.algorithm;

/** A $(I procedure) is a sequence of instructions that modifies
 *  the state of some objects.
 */
template isProcedure(alias T)
{
    enum bool isProcedure = std.traits.isCallable!T;
}

///
unittest
{
    //static assert ( !isProcedure!int );
    int i;
    static assert ( !isProcedure!i );
}

///
unittest
{
    auto sum(int a, int b) { return a + b; }

    static assert ( isProcedure!sum );
}

///
unittest
{
    auto sum(T)(T a, T b) { return a + b; }

    static assert ( isProcedure!(sum!int) );
}

///
unittest
{
    struct S { /*static*/ int opCall(int) { return 0; } }
    static assert ( isProcedure!S );

    S s;
    static assert ( isProcedure!s );
}

///
unittest
{
    struct S { int fun(int) { return 0; } }
    static assert ( isProcedure!(S.fun) );
}

/** A procedure is $(I regular) if its output depends only 
 *  on its inputs.
 */
template isRegularProcedure(alias T)
    if ( isProcedure!T )
{
    enum auto functionAttributes = [ __traits(getFunctionAttributes, T)[] ];
    enum bool isPure = canFind(functionAttributes, "pure");
// TODO check that all arguments are const or immutable refs, or values
//ref comes with attribute of the function (see template functionAttributes).
// TODO member function? check attr const|immutable?

    enum bool isRegularProcedure = isPure;
}

///
unittest
{
    int i;
    static assert ( !__traits(compiles, isRegularProcedure!i) );
}

///
unittest
{
    pure auto sum(int a, int b) { return a + b; }
    //pragma(msg, __traits(getFunctionAttributes, sum));

    static assert ( isRegularProcedure!sum );
}

///
unittest
{
    import std.stdio;
    auto sum(int a, int b) { writeln("dirty not pure"); return a + b; }
    //pragma(msg, __traits(getFunctionAttributes, sum));

    static assert ( !isRegularProcedure!sum );
}

///
unittest
{
    import std.stdio;
    struct S {
        int i;
        /*pure*/ int fun(int) { return 0; }
        pure int dirty(int) { return i++; }
        // does not compile pure int dirty2(int) { writeln("dirt"); return i++; }
        static int staticFun(int) { return 0; }
    }
    //pragma(msg, __traits(getFunctionAttributes, S.fun));
    //pragma(msg, __traits(getFunctionAttributes, S.dirty));
    //pragma(msg, __traits(getFunctionAttributes, S.dirty2));
    //pragma(msg, __traits(getFunctionAttributes, S.staticFun));

    static assert ( !isRegularProcedure!(S.fun) );
}

/// Alias to use std.traits.arity
alias arity = std.traits.arity;

/** A $(I funtional procedure) is a regular procedure
 *  with number or inputs greater than 0 and a single output.
 */
template isFunctionalProcedure(alias T)
    if ( isRegularProcedure!T )
{
    enum bool arityGreaterThan0 = dig.traits.procedure.arity!T > 0;
    static if ( !arityGreaterThan0 )
    {
        //pragma(msg, "functional procedure must have arity > 0");
        enum bool isFunctionalProcedure = false;
    }
    else static if (is(ReturnType!T == void))
    {
        enum bool isFunctionalProcedure = false;
    }
    else
    {
// no out, inout args
// no non const|immputable refs

        enum bool isFunctionalProcedure = true;
    }
}

///
unittest
{
    pure auto fun() { return 0; }

    static assert ( !isFunctionalProcedure!fun );
}

///
unittest
{
    pure auto sum(int a, int b) { return a + b; }

    static assert ( isFunctionalProcedure!sum );
}


///
unittest
{
    pure void fun(int a, int b) { auto c = a + b; }
    static assert (is(ReturnType!fun == void));

    static assert ( !isFunctionalProcedure!fun );
}

