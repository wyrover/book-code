#ifndef PARSE_HPP_
#define PARSE_HPP_

#include <istream>
#include <locale>
#include <ostream>
#include <stdexcept>
#include <string>

/** Exception class for parser errors, usually syntax errors. */
class parse_error : public std::runtime_error {
public:
  parse_error(std::string const& msg) : runtime_error{msg} {}
};

/** Parser class template. */
class parser
{
public:
  /** Constructor. Save the @p input stream.
   * @param input The input stream
   */
  parser(std::istream& input);

  /** Read one expression and store the result in @p result.
   * @param result Where to store the result of the expression.
   * @return true to continue or false to end the loop
   * @throw parse_error for various syntax and other errors
   */
  bool get_expr(double& result);

private:
  /** Convert a characer to a readable form.
   * @param c The character
   * @return A C++-style character literal that ensures @p c is readable.
   */
  std::string charify(char c);
  /** Parse a number.
   * @param result Store the number here
   * @return true to continue parsing or false to stop (end of file or error)
   */
  bool get_number(double& result);
  /** Parse a primary expression.
   * A primary is a parenthesized expression or a numeric literal.
   * @param result Store the number here
   * @return true to continue parsing or false to stop (end of file or error)
   */
  bool get_primary(double& result);
  /** Parse a multiplicative expression.
   * @param result Store the number here
   * @return true to continue parsing or false to stop (end of file or error)
   */
  bool get_mult_expr(double& result);

  std::istream& input_;
  std::ctype<char> const& ctype_;
};

/** Parse loop.
 * Read expressions from @p input and print results to @p output.
 * @param input The input stream.
 * @param output The output stream.
 */
void parse_loop(std::istream& input, std::ostream& output);

#endif
