/**
 * Time.
 *
 * Authors:   Igor Lesik 2015
 * Copyright: (C) Igor Lesik
 *
 * License:
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */
module cgem.d.physics.time;

import cgem.d.physics.realnum;
import cgem.d.physics.quantity;

enum Unit : ubyte {
    sec,  // second
    min,  // 60 sec
    hour, // 60 min
    day   // 24h
}

pure Real min2sec(Real min) { return min * 60; }
pure Real hour2min(Real hour) { return hour * 60; }
pure Real day2hour(Real day) { return day * 24; }
pure Real hour2sec(Real hour) { return hour.hour2min.min2sec; }
pure Real day2sec(Real day) { return day.day2hour.hour2sec; }

static immutable
Real[Unit.max+1][Unit.max+1] convMatrix=
[
    //        sec           min         hour      day
    /*sec*/   [1.0,        1.0/60,  1.0/(60*60),  1.0/(24*60*60)],
    /*min*/   [60.0,       1.0,     1.0/60,       1.0/(24*60)],
    /*hr */   [60.0*60.0,  60.0,    1.0,          1.0/24],
    /*day*/   [24*3600.0,  24*60.0, 24.0,         1.0]
];

pure Real convertUnit(Unit from, Unit to)
{
    return convMatrix[from][to];
}

unittest
{
    static assert( convertUnit(Unit.min, Unit.sec) == min2sec(1) );
    static assert( convertUnit(Unit.day, Unit.sec) == day2sec(1) );
}

template generateProp(string name)
{
    const char[] generateProp =
        "@property Real "~name~" (Real val) { unit = Unit."~name~"; return value = val; }"
       ~"@property Real "~name~"() { return value * convertUnit(unit, Unit."~name~"); }"
        ;
}

public struct Time
{
    enum quantity = dig.physics.quantity.Time;

public:
    Real value;
    Unit unit;

public:
    this(Real value, Unit unit) {this.value = value; this.unit = unit;}

public:

    mixin( generateProp!"sec" );
    mixin( generateProp!"min" );
    mixin( generateProp!"hour" );
    mixin( generateProp!"day" );

    Real changeUnit(Unit newUnit) {
        value *= convertUnit(unit, newUnit);
        unit = newUnit;
        return value;
    }

}

