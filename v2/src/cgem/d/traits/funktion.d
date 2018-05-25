/**
 * Functional Procedures.
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
 */
module dig.traits.funktion;

import std.traits;
import std.algorithm.searching;
import dig.traits.procedure;

/// Unary function has single input argument.
template isUnaryFunction(alias F)
{
    enum bool isUnaryFunction = isFunctionalProcedure!F && arity!F == 1;
}

///
unittest
{
    auto sum(int a, int b) { return a + b; }
    static assert ( !isUnaryFunction!sum );

    auto twice(int a) { return 2*a; }
    static assert ( isUnaryFunction!twice );
}

/// Check if all types in TypeTuple are the same.
template isAllTypesTheSame(TL...)
{
    static if (TL.length <= 1 )
    {
        enum isAllTypesTheSame = true;
    }
    else static if ( !is(TL[0] == TL[1]) )
    {
        enum isAllTypesTheSame = false;
    }
    else
    {
        enum isAllTypesTheSame = isAllTypesTheSame!(TL[1..$]);
    }
}

/// Alias for function parameters tuple.
alias procedureParameters = std.traits.Parameters;

/** Check if all parameters have the same type.
 *
 *  (∀ i,j ∈ N)(i,j < Arity(F)) ⇒ (InputType(F,i) = InputType(F,j))
 */
template isAllParametersTheSameType(alias F)
    if ( arity!F > 0 )
{
    alias args = procedureParameters!F;

    static if ( arity!F == 1 )
    {
        enum bool isAllParametersTheSameType = true;
    }
    else static if ( is(args[0] == args[1]) )
    {
        enum bool isAllParametersTheSameType = isAllTypesTheSame!args;
    }
    else
    {
        enum bool isAllParametersTheSameType = false;
    }
}

///
unittest
{
    auto sum(int a, int b, int c) { return a + b + c; }
    static assert ( isAllParametersTheSameType!sum );

    auto fun(int a, float b) { return 0; }
    static assert ( !isAllParametersTheSameType!fun );
}

/// All arguments of a homogeneous function have the same type.
template isHomogeneousFunction(alias F)
{
    enum bool isHomogeneousFunction =
        isFunctionalProcedure!F
     && arity!F > 0
     && isAllParametersTheSameType!F
    ;
}

///
unittest
{
    auto sum(int a, int b, int c) { return a + b + c; }
    static assert ( isHomogeneousFunction!sum );

    auto fun(int a, float b) { return 0; }
    static assert ( !isHomogeneousFunction!fun );
}

/// Codomain is function output type.
template codomain(alias F)
    //if ( isFunctionalProcedure!F )
{
    alias codomain = ReturnType!F;
}

///
unittest
{
    pure int fun(int a) { return a; }

    static assert ( is(codomain!fun == int) );
}

/// Function domain is domain where its inputs defined.
template domain(alias F)
    if ( /*isFunctionalProcedure!F*/ arity!F > 0 )
{
    alias domain = procedureParameters!F [0]; //TODO FIXME
}

///
unittest
{
    pure int fun(float a) { return 0; }

    static assert ( is(domain!fun == float) );
}
