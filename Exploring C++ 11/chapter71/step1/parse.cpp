#include <sstream>
#include <utility>

#include "parse.hpp"

parser::parser(std::istream& input)
: input_(input),
  ctype_(std::use_facet<std::ctype<char>>(input.getloc()))
{}

std::string parser::charify(char c)
{
  if (c == '\a') return R"('\a')";
  if (c == '\b') return R"('\b')";
  if (c == '\f') return R"('\f')";
  if (c == '\n') return R"('\n')";
  if (c == '\r') return R"('\r')";
  if (c == '\t') return R"('\t')";
  if (c == '\v') return R"('\v')";
  if (c == '\'') return R"('\'')";
  if (c == '\\') return R"('\\')";

  if (isprint(c))
    return std::string{"\'"} + std::string(1,c) + "\'";
  else {
    std::ostringstream stream{};
    stream << "'\\x" << std::hex;
    stream.fill('0');
    stream.width(2);
    stream << (std::char_traits<char>::to_int_type(c) & 0xFF) << '\'';
    return stream.str();
  }
}

/* Parse a floating point number.
 * NUMBER ::= SIGN? DIGIT+ ('.' DIGITS+)? ('E' SIGN? DIGITS+)?
 */
bool parser::get_number(double& result)
{
  std::string token{};
  char c{};
  if (not input_.get(c))
    return false;
  if (c == '+' or c == '-') {
    token += c;
    if (not input_.get(c))
      throw parse_error{"unterminated number: expected a digit after the sign"};
  }
  if (c < '0' or c > '9') {
    input_.unget();
    throw parse_error{"syntax error: expected digit, got " + charify(c)};
  }
  while (c >= '0' and c <= '9') {
    token += c;
    if (not input_.get(c)) {
      std::istringstream tmp{std::move(token)};
      // If the value overflows, return false.
      return (tmp >> result);
    }
  }
  if (c == '.') {
    token += c;
    if (not input_.get(c))
      throw parse_error{"unterminated number: expected digit after the decimal point"};
    if (c < '0' or c > '9') {
      input_.unget();
      throw parse_error{"syntax error: expected digit after decimal point, got " + charify(c)};
    }
    while (c >= '0' and c <= '9') {
      token += c;
      if (not input_.get(c)) {
        std::istringstream tmp{std::move(token)};
        // If the value overflows or is otherwise invalid, return false.
        return (tmp >> result);
      }
    }
  }
  if (c == 'e' or c == 'E') {
    token += c;
    if (not input_.get(c))
      throw parse_error{"unterminated number: expected digit in the exponent"};
    if (c == '-' or c == '+') {
      token += c;
      if (not input_.get(c))
        throw parse_error{"unterminated number: expected digit after sign in the exponent"};
    }
    if (c < '0' or c > '9') {
      input_.unget();
      throw parse_error{"syntax error: expected digit in the exponent, got " + charify(c)};
    }
    while (c >= '0' and c <= '9') {
      token += c;
      if (not input_.get(c)) {
        std::istringstream tmp{std::move(token)};
        // If the value overflows or is otherwise invalid, return false.
        return (tmp >> result);
      }
    }
  }
  input_.unget();

  std::istringstream tmp{std::move(token)};
  // If the value overflows or is otherwise invalid, return false.
  return (tmp >> result);
}

/* Parse an expression.
 * EXPR ::= MULT_EXPR | EXPR + MULT_EXPR | EXPR - MULT_EXP
 */
bool parser::get_expr(double& result)
{
  if (not get_mult_expr(result))
    return false;
  char c{};
  while (input_ >> c) {
    if (c != '+' and c != '-') {
      input_.unget();
      return true;
    } else {
      double right{};
      if (not get_mult_expr(right))
        throw parse_error{"syntax error: unterminated expression. Expected a multiplicative-exprssion after " + std::string(c,1)};
      if (c == '+')
        result += right;
      else
        result -= right;
    }
  }
  return true;
}

/* Parse a multiplicative-expression.
 * MULT_EXPR ::= PRIMARY | MULT_EXPR + PRIMARY | MULT_EXPR - PRIMARY
 */
bool parser::get_mult_expr(double& result)
{
  if (not get_primary(result))
    return false;
  char c{};
  while (input_ >> c) {
    if (c != '*' and c != '/') {
      input_.unget();
      return true;
    } else {
      double right{};
      if (not get_primary(right))
        throw parse_error{"syntax error: unterminated expression. Expected a primary after " + std::string(c,1)};
      if (c == '*')
        result *= right;
      else if (right == 0.0)
        throw parse_error{"division by zero"};
      else
        result /= right;
    }
  }
  return true;
}

/* Parse a primary.
 * PRIMARY ::= NUMBER | '(' EXPR ')'
 */
bool parser::get_primary(double& result)
{
  char c{};
  if (not (input_ >> c))
    // Can't read one character, so must be end-of-file
    return false;
  else if (c == '(') {
    if (not get_expr(result))
      return false;
    if (not (input_ >> c))
      throw parse_error{"syntax error: EOF when expecting ')'"};
    else if (c != ')')
      throw parse_error{"syntax error: expected ')', but got " + charify(c)};
    else
      return true;
  } else {
    input_.unget();
    return get_number(result);
  }
}

void parse_loop(std::istream& input, std::ostream& output)
{
  std::string line;
  // No portable way to test whether the console is an interactive terminal
  // vs. a non-interactive file. If you have a system-specific way to test,
  // output the prompt only for the interactive case.
  for (output << "> "; std::getline(input, line); output << "> ") {
    std::istringstream input{std::move(line)};
    parser p{input};
    try {
      double x{};
      while (p.get_expr(x))
        output << x << '\n';
    } catch(parse_error const& ex) {
      output << ex.what() << '\n';
    } catch(std::exception const& ex) {
      output << "exception: " << ex.what() << '\n';
    }
  }
}
