#pragma once

constexpr unsigned int factorial( unsigned int i )
{
  return (i == 0) ?             // terminal condition
         1u :                   // and terminal value
         i * factorial(i - 1u); // recursive definition
}
 
constexpr int factorial( int i )
{
  return (i < 0) ?            // error condition
         throw exception() :  // error reporting (at compile-time!)
         factorial((unsigned int)i);        // real computation
}
