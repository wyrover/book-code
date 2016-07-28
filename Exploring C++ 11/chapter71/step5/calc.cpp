#include <iostream>
#include <sstream>
#include <string>

#include "parse.hpp"

/** @mainpage
 * This is step 5 of the calculator project in Exploration 68 of <em>Exploring C++:
 * The Programmer's Introduction to C++</em>.
 *
 * This version of the calculator adds library files. The @c save command saves the
 * global symbol table to a file, and the @c load command loads symbols from a file
 * into the global symbol table.
 *
 * This calculator has a simple syntax:
 * @verbatim
   start ::= statement-list
   statement-list ::= empty | statement-list statement
   statement ::= definition | quit | expression | "load" string | "save" string
   definition ::= "def" identifier optional-parameters "=" expression
   optional-parameters ::= empty | "(" optional-identifier-list ")"
   optional-identifier-list ::= empty | identifier-list
   identifier-list ::= identifier | identifier-list "," identifier
   expression ::= multiplicative-expression "+" multiplicative-expression |
                  multiplicative-expression "-" multiplicative-expression
   multiplicative-expression ::= unary-expression "*" unary-expression |
                                 unary-expression "/" unary-expression
   unary-expression ::= "-" primary | "+" primary | primary
   primary ::= number | function-call | "(" expression ")"
   function-call ::= identifier optional_arguments
   optional_arguments ::= empty | "(" optional-expression-list ")"
   optional-expression-list ::= empty | expression-list
   expression-list ::= expression | expression-list "," expression
   @endverbatim
 *
 * Function arguments are evaluated lazily. There are no variables, only functions,
 * but function definitions can look just like variable assignments:
 * @verbatim
   def pi = 3.141592653589792
   @endverbatim
 * so there really isn't any difference between a variable and a function.
 *
 * The calculator reads expressions from the standard input, evaluates them,
 * and prints the results to the standard output. It's that simple.
 *
 * The calculator parses expression into a parse tree. The parser uses
 * node objects. The node class uses the pimpl idiom with a tree of
 * node_impl-derived classes.
 */
int main()
{
  parse_loop(std::cin, std::cout);
}
