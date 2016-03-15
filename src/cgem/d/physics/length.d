/**
 * Length.
 *
 * Authors:   Igor Lesik 2015
 * Copyright: (C) Igor Lesik
 *
 * License:
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */
module cgem.d.physics.length;

import cgem.d.physics.realnum;
import cgem.d.physics.quantity;

enum Unit : ubyte {
    m,    // metre, meter
    inch, // 0.0254 metre
    yard, // 0.9144 metre
    mile, // 1.609344 km
    ft    // 0.3048 metre
}

pure Real in2m(Real inch) { return inch * 0.0254; }
pure Real yd2m(Real yard) { return yard * 0.9144; }
pure Real mi2m(Real mile) { return mile * 1609.344; }
pure Real ft2m(Real feet) { return feet * 0.3048; }

public struct Length
{
    enum quantity = cgem.d.physics.quantity.Length;

public:
    Real value;
    Unit unit;

public:
    this(Real value, Unit unit) {this.value = value; this.unit = unit;}

public:
    @property Real m   (Real    m) { unit = Unit.m;    return value = m; }
    @property Real inch(Real inch) { unit = Unit.inch; return value = inch; }
    @property Real yard(Real yard) { unit = Unit.yard; return value = yard; }
    @property Real mile(Real mile) { unit = Unit.mile; return value = mile; }
    @property Real   ft(Real   ft) { unit = Unit.ft;   return value = ft; }


    @property Real m() { final switch (unit) {
        case Unit.m:    return value;
        case Unit.inch: return in2m(value);
        case Unit.yard: return yd2m(value);
        case Unit.mile: return mi2m(value);
        case Unit.ft:   return ft2m(value);
    }}


    @property Real convertToMetre() { return m(m); }
    //@property Real convertToInch()  { return inch(inch); }
}

public struct Area
{
    enum quantity = cgem.d.physics.quantity.Area;

public:
    Real value;
    Unit unit;

public:
    this(Real value, Unit unit) {this.value = value; this.unit = unit;}

public:
    @property Real m   (Real    m) { unit = Unit.m;    return value = m; }
    @property Real inch(Real inch) { unit = Unit.inch; return value = inch; }
    @property Real yard(Real yard) { unit = Unit.yard; return value = yard; }
    @property Real mile(Real mile) { unit = Unit.mile; return value = mile; }
    @property Real   ft(Real   ft) { unit = Unit.ft;   return value = ft; }


    @property Real m() { final switch (unit) {
        case Unit.m:    return value;
        case Unit.inch: return value.in2m.in2m;
        case Unit.yard: return value.yd2m.yd2m;
        case Unit.mile: return value.mi2m.mi2m;
        case Unit.ft:   return value.ft2m.ft2m;
    }}


    @property Real convertToMetre() { return m(m); }
    //@property Real convertToInch()  { return inch(inch); }

}

Area make(Length l1, Length l2)
{
    return Area(l1.m * l2.m, Unit.m);
}

unittest
{
    static assert (Area(0.0254*0.0254,Unit.m).m == Area(1, Unit.inch).m);

    static assert (Area(2*3, Unit.m).m == make(Length(2, Unit.m), Length(3, Unit.m)).m);
}

public struct Volume
{
    enum quantity = cgem.d.physics.quantity.Volume;

public:
    Real value;
    Unit unit;

public:
    this(Real value, Unit unit) {this.value = value; this.unit = unit;}

public:
    @property Real m   (Real    m) { unit = Unit.m;    return value = m; }
    @property Real inch(Real inch) { unit = Unit.inch; return value = inch; }
    @property Real yard(Real yard) { unit = Unit.yard; return value = yard; }
    @property Real mile(Real mile) { unit = Unit.mile; return value = mile; }
    @property Real   ft(Real   ft) { unit = Unit.ft;   return value = ft; }


    @property Real m() { final switch (unit) {
        case Unit.m:    return value;
        case Unit.inch: return value.in2m.in2m.in2m;
        case Unit.yard: return value.yd2m.yd2m.yd2m;
        case Unit.mile: return value.mi2m.mi2m.mi2m;
        case Unit.ft:   return value.ft2m.ft2m.ft2m;
    }}


    @property Real convertToMetre() { return m(m); }
    //@property Real convertToInch()  { return inch(inch); }
}

unittest
{
    static assert (Volume(0.0254*0.0254*0.0254,Unit.m).m == Volume(1, Unit.inch).m);
}
