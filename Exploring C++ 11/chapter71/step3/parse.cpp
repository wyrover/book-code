#include <sstream>

#include "node.hpp"
#include "parse.hpp"

parser::parser(std::istream& input)
: input_(input),
  ctype_(std::use_facet<std::ctype<char> >(input.getloc())),
  token_(),
  kind_()
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

void parser::get_identifier(std::string& identifier)
{
  identifier.clear();
  char c{};
  if (not input_.get(c))
    return;
  if (not isalpha(c))
    throw parse_error{"syntax error: expected alphabetic, got " + charify(c)};
  identifier += c;
  while (input_.get(c)) {
    if (not isalnum(c)) {
      input_.unget();
      return;
    }
    identifier += c;
  }
  return;
}

parser::kind parser::get_token(std::string& token)
{
  if (not token_.empty())
  {
    token = token_;
    kind result(kind_);
    token_.clear();
    kind_ = eof;
    return result;
  }

  char c{};
  if (not (input_ >> c))
    return eof;
  if (isalpha(c)) {
    input_.unget();
    get_identifier(token);
    return identifier;
  }

  // Get a numeric literal.
  token.clear();
  if (c == '+' or c == '-' or c == '*' or c == '/' or c == '%' or c == '(' or c == ')' or c == '=') {
    token += c;
    return static_cast<kind>(c);
  }

  if (c < '0' or c > '9') {
    input_.unget();
    throw parse_error{"syntax error: expected digit, got " + charify(c)};
  }
  while (c >= '0' and c <= '9') {
    token += c;
    if (not input_.get(c))
      return number;
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
      if (not input_.get(c))
        return number;
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
      if (not input_.get(c))
        return number;
    }
  }
  input_.unget();
  return number;
}

bool parser::get_number(std::string const& token, node& result)
{
  std::istringstream stream{token};
  // If the value overflows or is otherwise invalid, return false.
  double value;
  if (not (stream >> value))
    return false;
  result = node(value);
  return true;
}

/* Parse an expression.
 * EXPR ::= "var" IDENTIFIER ":=" ADD_EXPR | ADD_EXPR
 */
bool parser::get_expr(node& result)
{
  std::string token{};
  kind k(get_token(token));
  if (k == eof)
    return false;

  if (k == identifier and token == "var") {
    std::string name{};
    // Define a variable.
    k = get_token(name);
    if (k != identifier)
      throw parse_error{"syntax error: expected IDENTIFIER, but got " + name};
    k = get_token(token);
    if (k != '=')
      throw parse_error{"syntax error: expected =, but got " + token};
    if (not get_add_expr(result))
      throw parse_error{"syntax error: expected additive-exprssion in assignment"};
    result = node(node{std::move(name)}, result);
    return true;
  }

  if (k == identifier and token == "quit")
    std::exit(0);

  push_back(token, k);
  if (not get_add_expr(result))
    throw parse_error{"syntax error: expected an additive-expression"};

  return true;
}

/* Parse an addition expression
 * ADD_EXPR ::= MUL_EXPR | ADD_EXPR + MUL_EXPR | ADD_EXPR - MUL_EXPR
 */
bool parser::get_add_expr(node& result)
{
  if (not get_mul_expr(result))
    return false;
  std::string token{};
  while (kind k = get_token(token)) {
    if (k != '+' and k != '-') {
      push_back(token, k);
      return true;
    } else {
      node right{};
      if (not get_mul_expr(right))
        throw parse_error{"syntax error: unterminated expression. Expected a multiplicative-expression after " + token};
      result = node(result, k, right);
    }
  }
  return true;
}

/* Parse a multiplicative expression.
 * MUL_EXPR ::= UNARY | MUL_EXPR + UNARY | MUL_EXPR - UNARY
 */
bool parser::get_mul_expr(node& result)
{
  if (not get_unary(result))
    return false;
  std::string token{};
  while (kind k = get_token(token)) {
    if (k != '*' and k != '/') {
      push_back(token, k);
      return true;
    } else {
      node right{};
      if (not get_unary(right))
        throw parse_error{"syntax error: unterminated expression. Expected a unary-expression after " + token};
      result = node(result, k, right);
    }
  }
  return true;
}

/* Parse a unary expression.
 * UNARY ::= '-' PRIMARY | '+' PRIMARY | PRIMARY
 */
bool parser::get_unary(node& result)
{
  std::string token{};
  if (kind k = get_token(token)) {
    if (k == '-') {
      if (not get_primary(result))
        return false;
      result = node(k, result);
      return true;
    } else if (k == '+') {
      return get_primary(result);
    } else {
      push_back(token, k);
      return get_primary(result);
    }
  }
  return false;
}

/* Parse a primary expression.
 * PRIMARY ::= NUMBER | IDENTIFIER | '(' EXPR ')'
 */
bool parser::get_primary(node& result)
{
  std::string token{};
  if (kind k = get_token(token)) {
    if (k == '(') {
      if (not get_expr(result))
        return false;
      k = get_token(token);
      if (k == eof)
        throw parse_error{"syntax error: EOF when expecting ')'"};
      else if (k != ')')
        throw parse_error{"syntax error: expected ')', but got " + token};
      else
        return true;
    } else if (k == number) {
      if (not get_number(token, result))
        throw parse_error{"Invalid numeric literal: " + token};
      return true;
    } else if (k == identifier) {
      result = node{std::move(token)};
      return true;
    } else {
      throw parse_error{"syntax error: expected a primary, but got " + token};
    }
  }
  return false;
}

void parse_loop(std::istream& input, std::ostream& output)
{
  std::string line{};
  // No portable way to test whether the console is an interactive terminal
  // vs. a non-interactive file. If you have a system-specific way to test,
  // output the prompt only for the interactive case.
  for (output << "> "; std::getline(input, line); output << "> ") {
    std::istringstream input{std::move(line)};
    parser p{input};
    try {
      node n;
      while (p.get_expr(n))
        output << n.evaluate() << '\n';
    } catch(parse_error const& ex) {
      output << ex.what() << '\n';
    } catch(std::exception const& ex) {
      output << "exception: " << ex.what() << '\n';
    }
  }
}
