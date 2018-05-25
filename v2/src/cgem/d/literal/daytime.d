/**
 * Time of day literal.
 *
 * Usage is `auto timeInSec = daytime!"1:15:33"`.
 *
 * Authors:   Igor Lesik 2014-2015
 * Copyright: (C) Igor Lesik
 *
 * License:
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See_Also:
 * "D Cookbook" by Adam D. Ruppe, chapter 9.
 * See_Also:
 * $(LINK http://www.drdobbs.com/tools/user-defined-literals-in-the-d-programmi/229401068)
 */
module dig.literal.daytime;

import std.array;
import std.algorithm;
import std.conv;

/** 
 * Read "time" string and convert it to # of seconds since midnight.
 *
 * Params:
 * time_str = string representing time of day.
 *
 * Returns: number of seconds since midnight.
 */
uint readDaytimeString(string time_str)
{
    uint check(uint n) {if (n>60) throw new Exception("bad time "); return n;}

    uint sum =
        time_str
       .split(":")                  // find all tokens
       .map!(to!uint)               // convert each token to number
       .map!(check)                 // check that number is sane, less than 61
       .reduce!((a,b) => a*60 + b)  // sum the numbers
       ;

    if (sum > 60*60*24) throw new Exception("bad time");

    return sum;
}

version(unittest)
{
    import std.stdio;
    import std.exception;
}

unittest
{
    writeln("Testing module:" ~ __MODULE__);
}

/// Test readDaytimeString()
unittest
{
    static assert(readDaytimeString("00:00:00") == 0);
    static assert(readDaytimeString("02:01:01") == 2*60*60 + 61);

    assertThrown!Exception(readDaytimeString("0i:0m:j"));

    assertThrown!Exception(readDaytimeString("01:61:00"));

    assertThrown!Exception(readDaytimeString("24:00:01"));
}

/**
 * Convert day time string literal to number literal.
 *
 * Using template+enum forces CTFE. When you declare an `enum` structure
 * with a value, the right-hand side of the equation is immediatly evaluated
 * at compile time, regardless of complexity.
 */
template daytime(string s)
{
    enum daytime = readDaytimeString(s);
}

/// Test daytime
unittest
{
    static assert(daytime!"1:15:33" == 1*60*60 + 15*60 + 33);
    static assert(daytime!"1:15:33" - daytime!"1:15:00" == 33);
}

/// Makes sense to have template specification with `uint` also.
template daytime(uint time_in_sec)
{
    enum daytime = time_in_sec;
    static assert(time_in_sec <= daytime!"24:00:00");
}

///
unittest
{
    static assert(daytime!"00:00:00" + daytime!61 == daytime!"1:01");
}

///
template am(uint time)
{
    enum am = time;
    static assert(time <= daytime!"12:00:00");
}

///
template pm(uint time)
{
    enum pm = time + daytime!"12:00:00";
    static assert(time <= daytime!"12:00:00");
}

///
unittest
{
    static assert(am!(daytime!"3:45:2") == daytime!"3:45:2");
    static assert(pm!(daytime!"3:45:2") == daytime!"15:45:2");

}

