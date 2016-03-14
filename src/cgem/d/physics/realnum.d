/**
 * Version/size of real number.
 *
 * Authors:   Igor Lesik 2015
 * Copyright: (C) Igor Lesik
 *
 * License:
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */
module cgem.d.physics.realnum;

version (real80)
{
    alias Real = real;
}
else
{
    alias Real = double;
}

