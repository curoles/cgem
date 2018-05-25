/**
 * Temperature.
 *
 * Authors:   Igor Lesik 2015
 * Copyright: (C) Igor Lesik
 *
 * License:
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See_Also:
 * https://en.wikipedia.org/wiki/List_of_physical_quantities
 */
module cgem.d.physics.temperature;

import cgem.d.physics.realnum;
import cgem.d.physics.quantity;

enum Real KCscaleOffset = 273.15;
enum Real FCscaleOffset = 32;

pure
Real FtoC(in Real temperatureF)
{
    return ((temperatureF - FCscaleOffset)*5)/9;
}

pure
Real CtoF(in Real temperatureC)
{
    return ((temperatureC + FCscaleOffset)*9)/5;
}

pure
Real KtoC(in Real temperatureK)
{
    return temperatureK - KCscaleOffset;
}

pure
Real CtoK(in Real temperatureC)
{
    return temperatureC + KCscaleOffset;
}

pure
Real FtoK(in Real temperatureF)
{
    return temperatureF.FtoC.CtoK;
}

pure
Real KtoF(in Real temperatureK)
{
    return temperatureK.KtoC.CtoF;
}

pure
Real XtoX(in Real temp)
{
    return temp;
}

enum Unit : ubyte {K, C, F}

alias ConvFun = pure Real function(in Real);

static immutable
ConvFun[Unit.max+1][Unit.max+1] convMatrix=
[
    //        K           C         F
    /*K*/   [&XtoX,     &KtoC,   &KtoF],
    /*C*/   [&CtoK,     &XtoX,   &CtoF],
    /*F*/   [&FtoK,     &FtoC,   &XtoX]
];

pure Real convertValue(Real value, Unit from, Unit to)
{
    return convMatrix[from][to](value);
}

template generateProp(string name)
{
    const char[] generateProp =
        "@property Real "~name~" (Real val) { unit = Unit."~name~"; return value = val; }"
       ~"@property Real "~name~"() { return convertValue(value, unit, Unit."~name~"); }"
        ;
}

public struct Temperature
{
    enum quantity = cgem.d.physics.quantity.Temperature;

public:
    Real value;
    Unit unit;

public:

    mixin( generateProp!"K" );
    mixin( generateProp!"C" );
    mixin( generateProp!"F" );


    @property Real changeToK() { return K(K); }
    @property Real changeToC() { return C(C); }
    @property Real changeToF() { return F(F); }

    Real changeUnit(Unit newUnit) {
        value.convertValue(unit, newUnit);
        unit = newUnit;
        return value;
    }

}

unittest
{
    //static assert (Temperature.sizeof == Real.sizeof + Unit.sizeof);
}
