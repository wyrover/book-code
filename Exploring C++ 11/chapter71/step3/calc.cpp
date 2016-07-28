#include <iostream>
#include <sstream>
#include <string>

#include "parse.hpp"

/** @mainpage
 * This is step 3 of the calculator project in Exploration 68 of <em>Exploring C++:
 * The Programmer's Introduction to C++</em>.
 *
 * This version of the calculator implements simple arithmetic. It uses a recursive-descent
 * parser to build a parse tree. Then it evaluates the parse tree and prints the result.
 *
 * The node.hpp file declares the parse tree node class, which uses the pimpl idiom to wrap
 * a pointer to a node_impl object. The node_impl class is the root of the parse tree
 * classes.
 */

int main()
{
  parse_loop(std::cin, std::cout);
}
