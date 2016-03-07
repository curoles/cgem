/**
 * @file
 * @brief Iterative SQRT
 * @author Igor Lesik
 */

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
