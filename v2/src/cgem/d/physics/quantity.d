/**
 * Measurable physical quantities.
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
module cgem.d.physics.quantity;

import std.typecons;


enum QuantityId : uint
{
    Length,
    Time,
    Mass,
    Temperature,

    Area,
    Volume,
    Velocity
}

alias Q = QuantityId;

alias Dimension = Tuple!(
    int, "L",
    int, "T",
    int, "M",
    int, "Temp"
);

pure Dimension combineDimension(string op)(in Dimension a, in Dimension b)
{
    Dimension c = a;
    foreach (field; Dimension.fieldNames) {
        mixin("c."~field~" "~op~"= b."~field~";");
    }
    return c;
}

pure Dimension mul(in Dimension a, in Dimension b)
{
    return combineDimension!"+"(a, b);
}

pure Dimension div(in Dimension a, in Dimension b)
{
    return combineDimension!"-"(a, b);
}

alias Quantity = Tuple!(QuantityId, "id", Dimension, "dim");

pure Dimension mul(in Quantity a, in Quantity b){
    return a.dim.mul(b.dim);
}

pure Dimension div(in Quantity a, in Quantity b){
    return a.dim.div(b.dim);
}

/*struct QuantityProxy
{
Quantity _q;
this(Quantity q) { _q = q; }
Dimension opBinary(string op)(Quantity rhs)
{
    static if (op == "*") return _q.mul(rhs.dim);
    else static if (op == "/") return _q.div(rhs.dim);
    else static assert(0, "Operator "~op~" not implemented");
}
}*/

enum Quantity Length = tuple(QuantityId.Length, tuple(1, 0, 0, 0));
enum Quantity Area   = tuple(QuantityId.Area,   tuple(2, 0, 0, 0));
enum Quantity Volume = tuple(QuantityId.Volume, Area.mul(Length));

unittest
{
    static assert (Length.id == QuantityId.Length);
    static assert (Volume.dim.L == 3);
}

enum Quantity Temperature  = tuple(QuantityId.Temperature, tuple(0, 0, 0, 1));

unittest
{
    static assert (Temperature.id == QuantityId.Temperature);
    static assert (Temperature.dim.Temp == 1 && Temperature.dim.L == 0);
}

enum Quantity Time = tuple(QuantityId.Time, tuple(0, 1, 0, 0));

enum Quantity Velocity = tuple(QuantityId.Velocity, Length.div(Time));

unittest
{
    static assert (Velocity.dim.L == 1 && Velocity.dim.T == -1);
}

