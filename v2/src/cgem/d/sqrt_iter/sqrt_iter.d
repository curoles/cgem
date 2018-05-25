/**
 * @file
 * @brief Iterative SQRT
 * @author Igor Lesik
 */
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


double sqrt_iterative(double number)
{
    ResultRange range = ResultRange(number);
	
    do
    {
        double sroot = range.middle;       // try middle of the range

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

