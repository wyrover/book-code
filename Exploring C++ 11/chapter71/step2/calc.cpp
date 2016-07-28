#include <iostream>
#include <sstream>
#include <string>

#include "parse.hpp"

/** @mainpage
 * This is step 2 of the calculator project in Exploration 68 of <em>Exploring C++:
 * The Programmer's Introduction to C++</em>.
 *
 * This version of the calculator implements simple arithmetic. It uses a recursive-descent
 * parser and immediately evaluates expressions using @c double for all values. It adds
 * variables and assignments to step 1.
 *
 * To declare a variable, use:
 * @verbatim
   var NAME = EXPRESSION
   @endverbatim
 */

int main()
{
  parse_loop(std::cin, std::cout);
}
