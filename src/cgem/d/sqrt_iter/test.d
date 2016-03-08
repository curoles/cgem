import std.conv;
import std.stdio;


void main(string[] args)
{
    double number = to!double(args[1]);
	
    double sqrtResult = iterativeSqrt(number);
	
    writefln("sqrt(%f)=%f", number, sqrtResult);
}

