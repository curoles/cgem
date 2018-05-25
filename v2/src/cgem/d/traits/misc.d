module dig.traits.misc;


import std.traits;
version(unittest)
{
    import std.stdio;
}



/** Identity, a primitive notion of our perception of reality,
 *  determines the sameness of a thing changing over time.
 *  Here we define identity as thing's name.
 */
string getIdentity(T...)()
{
    return std.traits.fullyQualifiedName!T;
}

unittest
{
    int i=2;
    struct S { int i; void foo(){}}
    S s;
    writeln(getIdentity!i);
    writeln(getIdentity!S);
    //writeln(getIdentity!(S.i));
    //writeln(getIdentity!(s.foo));
}

/*getEntityAttributes(T)()
{
    auto b = [ __traits(allMembers, T) ];
std.traits.Fields!T;
    writeln(b);
}*/

string getCurrentModuleName()
{
    return __MODULE__;
}

int member1;
class Member2 {};

unittest
{
    writeln(getCurrentModuleName());
struct S {int a; int b;}
writeln(std.traits.moduleName!(S));
writeln(std.traits.FieldNameTuple!(S));
//writeln(std.traits.Fields!(S));
string mname = __MODULE__;
writeln(__traits(allMembers, mixin(__MODULE__)));
}


version(none){
unittest
{
alias Alias(alias Symbol) = Symbol; // this does the trick

foreach (item; __traits(allMembers, mixin(__MODULE__)))
	{
		alias sym = Alias!(__traits(getMember, mixin(__MODULE__), item));
                //alias membr = __traits(getMember, mixin(__MODULE__), item);
		//static if (is(sym == class))
		{
			pragma(msg, item ~ ":" ~ fullyQualifiedName!sym ~ ":" ~ typeof(sym));
                        writeln(item);// ~ ":" ~ membr);//fullyQualifiedName!membr);
 		}
	}
}
}

//pragma(msg, import(.stringof[7..$] ~ ".d"));



