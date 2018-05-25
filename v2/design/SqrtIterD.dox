$(H2 Find square root)

$(P
Our first algorithm will be finding square
root of a number. The first step in writing an algorithm
is usually asking yourself how you solve the problem
in your head. We (at least most of us) do not have any quick
way to find a square root, compare to finding a sum of two
numbers for example. What we do to find a square root of number N is
that we guess a number M then multiply it by itself M*M and see
how close M*M is to the N. If we overshoot and M*M is greater
than N, then we pick another number K that is smaller than M.
Otherwise, if we undershoot and M*M is less than N, then
we pick number K that is greater than M. And so on until we close enough.
)

$(P
Nothing stops us from using the same algorithm for our first program.
In computer science such algorithm is called $(I iterative).
All we have to do is to get a bit closer to the target on each iteration,
eventually we will be close enough and that is when we stop the iterations.
True that people can do a reasonable guess what number M to pick first
in order for M$(POW 2) to be close to N, but it does not matter for
a computer algorithm that much since computer does calculations very fast.
What we can say for sure that first M should be greater than 0 and less
than N, that is it should belong to the range [0, N].
)

$(P
All right, our first M$(SUB 1) will be some number between 0 and N, for instance
we can choose the middle of the range [0, N] that is M$(SUB 1)=N/2.
Next we compare M$(SUB 1)$(POW 2) to N, the comparison will show that:
$(OL
$(LI either M$(SUB 1)$(POW 2) < N)
$(LI or M$(SUB 1)$(POW 2) > N)
$(LI or M$(SUB 1)$(POW 2) = N or close enough.)
)
In case #3 when  M$(SUB 1)$(POW 2) close enough to N we stop and report that
M$(SUB 1) is our solution for &radic;N.
)

$(P
In first case when M$(SUB 1)$(POW 2) < N our M$(SUB 1) is too small and we should choose
a greated number somewhere between M$(SUB 1) and N, that is in the range [M$(SUB 1), N].
Again, we can choose our next M$(SUB 2) as the middle of [M$(SUB 1), N],
that would be M$(SUB 2)=M$(SUB 1)+(N-M$(SUB 1))/2.
)

$(P
Similarly, in second case when M$(SUB 1)$(POW 2) > N our M$(SUB 1) is too big and we should choose
a smaller number somewhere between 0 and M$(SUB 1), that is in the range [0, M$(SUB 1)].
We choose our next M$(SUB 2) as the middle of [0, M$(SUB 1)],
that would be M$(SUB 2)=0+(M$(SUB 1)-0)/2.
)

$(P
In both cases we again deal with a range and choose next M$(SUB n) as its middle.
So step when we picked M$(SUB 2) is not different from the step we picked M$(SUB 1).
And nothing prevents from using the same tecnique over and over untill
we iterativly come to M$(SUB n) where M$(SUB n)$(POW 2) is close enough to N.  
)

$(P
Note that all we used to devise our algorithm was a simple range.
On each iteration we have a range and create a new one for the next iteration
by choosing either lower half of original range or its upper half.
You can say that on each iteration we bisecting original range.
)

$(P
Imagine for a minute that we already have a custom type called $(B ResultRange) that
provides an abstaction for the range. Say it has get and set properties
for such things as lower and upper boundaries of the range and also its middle.
Then we are ready to code the algorithm:
---
double iterativeSqrt(double number)
{
    ResultRange range = ResultRange(number);
	
    do
    {
        double sroot = range.middle; // try middle of the range

        if ( (sroot * sroot) >= number ) { // M^2 > N
            range.upper = sroot;           //   use lower half
        }                                  //   [L,U] → [L,L+(U-L)/2]
        else {                             // M^2 < N
            range.lower = sroot;           // use upper half
        }                                  //   [L,U] → [L+(U-L)/2,U]

    } // run untill precision of the solution is bigger than 0.1%
    while ( range.span/range.middle > 0.001 );

    return range.middle;
}
---
)

$(P
Line `ResultRange range = ResultRange(number);` defines variable `range`.
When the variable is constructed its range is [0, N] where N is number that
square root we are calculating.
Then goes `do {...} while ( range.span/range.middle > 0.001 );` loop; inside this
loop we iterativly shrink the range of possible solutions; while condition
says that we do not want to stop iterating untill size or the range is not
small enough compare to its middle point that is the solution,
in other words we want precision of the solution to be not less than 0.1%.
The code inside of the loop implements devised algorithm.
Each time we select the middle of the range `double sroot = range.middle;`.
Then we compare its square with N `if ( (sroot * sroot) >= number )`;
if the condition is true then we replace upper boundary of the range
with its middle, because we overshoot, otherwise we replace
lower boundary with middle point, becase we undershoot.
)

$(P
All is left for us to do is to code `struct ResultRange`.
This structure holds two data fields for lower and upper
range boundaries:
---
struct ResultRange
{
    double lowerBoundary, upperBoundary;
...
}
---
Get properties for these two values are trivial:
---
    @property double lower() { return lowerBoundary; }
    @property double upper() { return upperBoundary; }
---
Additionaly we define a property that returns size or span of the range:
---
@property double span() { return upperBoundary - lowerBoundary; }
---
and property that returns middle point of the range:
---
    @property double middle()
    in { assert (upperBoundary >= lowerBoundary); }
    body {
        return lowerBoundary + span/2;
    }
---

Set properties are pretty trivial too, we just assign new value either
to lower or upper boundary:
---
    @property double lower(double newBoundary)
    in { assert(newBoundary <= upperBoundary); }
    body {
        return lowerBoundary = newBoundary;
    }

    @property double upper(double newBoundary)
    in { assert(newBoundary >= lowerBoundary); }
    body {
        return upperBoundary = newBoundary;
    }
---

Proper initialization of the range is done by function `reset`
that is also called inside the constructor:
---
    void reset(double number) {
        lowerBoundary = 0;
        upperBoundary = number;
    }

    this(double number) { reset(number); }
---
)

$(P
Putting all pieces together we get the program that iterativly
finds an approximation for &radic;N.
---
import std.conv;
import std.stdio;

struct ResultRange
{
    double lowerBoundary, upperBoundary;

    void reset(double number) {
        lowerBoundary = 0;
        upperBoundary = number;
    }

    this(double number) { reset(number); }

    @property double lower() { return lowerBoundary; }
    @property double upper() { return upperBoundary; }

    @property double span() { return upperBoundary - lowerBoundary; }

    @property double middle()
    in { assert (upperBoundary >= lowerBoundary); }
    body {
        return lowerBoundary + span/2;
    }

    @property double lower(double newBoundary)
    in { assert(newBoundary <= upperBoundary); }
    body {
        return lowerBoundary = newBoundary;
    }

    @property double upper(double newBoundary)
    in { assert(newBoundary >= lowerBoundary); }
    body {
        return upperBoundary = newBoundary;
    }
}

double iterativeSqrt(double number)
{
    ResultRange range = ResultRange(number);

    uint iteration;
	
    do
    {
        writefln("%d [%f, %f]", iteration++, range.lower, range.upper);

        double sroot = range.middle;
        if ( (sroot * sroot) >= number ) {
            range.upper = sroot;
        }
        else {
            range.lower = sroot;
        }
    } while ( range.span/range.middle > 0.001 );

    return range.middle;
}


void main(string[] args)
{
    double number = to!double(args[1]);
	
    double sqrtResult = iterativeSqrt(number);
	
    writefln("sqrt(%f)=%f", number, sqrtResult);
}
---
)

$(P
Now the fun part, let us check how it works.
)
