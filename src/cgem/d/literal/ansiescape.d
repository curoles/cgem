//https://en.wikipedia.org/wiki/ANSI_escape_code
/**
 * String literals for ANSI Escape Codes,
 * particulary SGR (Select Graphic Rendition) color parameters.
 *
 * Usage example: `writeln(color!"red"  ~ "I am red " ~ reset);`.
 *
 * Authors:   Igor Lesik 2014-2015
 * Copyright: (C) Igor Lesik
 *
 * License:
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See_Also:
 * $(LINK https://en.wikipedia.org/wiki/ANSI_escape_code)
 */

module dig.literal.ansiescape;

import std.uni;
import std.conv;

version = AnsiColors;

version (AnsiColors){

/// ANSI escape string to reset all attributes.
enum reset = "\x1b[0m";

/// ANSI escape string to set bold font and/or bright color.
enum bold  = "\x1b[1m";

/// ANSI escape string to reset colors to defaults.
enum resetColors = "\x1b[39m\x1b[49m";

} else {// NO coloring

enum reset = "";
enum bold  = "";
enum resetColors = "";

}

/** 
 * Read color name string and convert it color index.
 *
 * Params:
 * color_str = color name string like "red|green|white".
 *
 * Returns: color index.
 */
uint convertColorStringToIndex(string color_str)
{
    enum Color : uint {
        Black = 0, Red, Green, Yellow, Blue,
        Magenta, Cyan, White, Illegal, Default
    }

    uint id = Color.Default;

    switch (color_str.toLower())
    {
        default:        id = Color.Default; break;
        case "black"  : id = Color.Black;   break;
        case "red"    : id = Color.Red;     break;
        case "green"  : id = Color.Green;   break;
        case "yellow" : id = Color.Yellow;  break;
        case "blue"   : id = Color.Blue;    break;
        case "magenta": id = Color.Magenta; break;
        case "cyan"   : id = Color.Cyan;    break;
        case "white"  : id = Color.White;   break;
    }

    return id;
}

version (AnsiColors){

/**
 * Convert color name string literal to ANSI escape code string literal.
 *
 * Using template+enum forces CTFE. When you declare an `enum` structure
 * with a value, the right-hand side of the equation is immediatly evaluated
 * at compile time, regardless of complexity.
 *
 * See Manifest Constants $(LINK http://dlang.org/enum.html)
 */
template color(string s)
{
    enum color = "\x1b[" ~ to!string(30 + convertColorStringToIndex(s)) ~ "m";
}

/// Background color
template bgcolor(string s)
{
    enum bgcolor = "\x1b[" ~ to!string(40 + convertColorStringToIndex(s)) ~ "m";
}

/// Both foreground and backgorund colors
template colors(string fore, string back)
{
    enum colors = color!(fore) ~ bgcolor!(back);
}

} else {// NO coloring

template color(string s){enum color = "";}
template bgcolor(string s){enum bgcolor = "";}
template colors(string fore, string back){enum colors = "";}

} // version(AnsiColors)

version(unittest)
{
    import std.stdio;
}

unittest
{
    writeln("Testing module:" ~ __MODULE__);
}

///
unittest
{
version (AnsiColors){
    static assert(color!"red" == "\x1b[31m");
}
    writeln(color!"red"  ~ "I am red " ~ color!"green" ~ "and I am green" ~ reset);
    writeln(bold ~ color!"yellow"  ~ "bold yellow " ~ color!"white" ~ "white" ~ reset);
    writeln(bold ~ colors!("yellow","blue") ~ "yellow on blue" ~ reset);
}

