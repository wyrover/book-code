#include <cstdlib>
#include <iterator>
#include <sstream>

#include "calc_error.hpp"
#include "node.hpp"
#include "parse.hpp"
#include "variables.hpp"

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
    throw syntax_error("expected alphabetic, got " + charify(c));
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

void parser::push_back(std::string const& token, kind k)
{
  kind_ = k;
  if (kind_ == eof)
    token_ = "end of line";
  else
    token_ = token;
}

parser::kind parser::get_token(std::string& token)
{
  if (not token_.empty())
  {
    kind result{kind_};
    token = token_;

    token_.clear();
    kind_ = eof;

    return result;
  }

  char c{};
  if (not (input_ >> c)) {
    token = "end of line";
    return eof;
  }
  if (isalpha(c)) {
    input_.unget();
    get_identifier(token);
    return identifier;
  }

  // Get a numeric literal.
  token.clear();
  if (c == '+' or c == '-' or c == '*' or c == '/' or c == '%' or c == '(' or c == ')' or c == '=' or c == ',') {
    token += c;
    return kind(c);
  }

  if (c < '0' or c > '9') {
    input_.unget();
    throw syntax_error{"expected digit, got " + charify(c)};
  }
  while (c >= '0' and c <= '9') {
    token += c;
    if (not input_.get(c))
      return number;
  }
  if (c == '.') {
    token += c;
    if (not input_.get(c))
      throw syntax_error{"unterminated number: expected digit after the decimal point"};
    if (c < '0' or c > '9') {
      input_.unget();
      throw syntax_error{"expected digit after decimal point, got " + charify(c)};
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
      throw syntax_error{"unterminated number: expected digit in the exponent"};
    if (c == '-' or c == '+') {
      token += c;
      if (not input_.get(c))
        throw syntax_error{"unterminated number: expected digit after sign in the exponent"};
    }
    if (c < '0' or c > '9') {
      input_.unget();
      throw syntax_error{"expected digit in the exponent, got " + charify(c)};
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
  double value{};
  if (not (stream >> value))
    return false;
  result = node(value);
  return true;
}

void parser::get_definition(std::string& name, identifier_list& parameters, node& definition)
{
  // Define a variable.
  kind k{get_token(name)};
  if (k != identifier)
    throw syntax_error{"expected IDENTIFIER, got " + name};

  std::string token;
  k = get_token(token);
  if (k == '(') {
    get_namelist(std::back_inserter(parameters));
    k = get_token(token);
  }

  if (k != '=')
    throw syntax_error{"expected = in definition, got " + token};

  if (not get_expr(definition))
    throw syntax_error{"expected exprssion in assignment"};
}

bool parser::get_statement(std::ostream& output)
{
  std::string token{};
  kind k(get_token(token));
  if (k == eof)
    return false;

  if (k == identifier and token == "def") {
    node definition{};
    identifier_list parameters{};
    get_definition(token, parameters, definition);
    set_function(token, node{std::move(parameters), definition});
    return true;
  }

  if (k == identifier and token == "quit")
    std::exit(0);

  // Otherwise, the statement must be an expression.
  push_back(token, k);
  node n{};
  if (not get_expr(n))
    return false;
  else {
    // Evaluate the expression and print the result.
    output << n.evaluate() << '\n';
    return true;
  }
}

bool parser::get_expr(node& result)
{
  return get_add_expr(result);
}

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
        throw syntax_error{"unterminated expression. Expected a multiplicative-expression after " + token};
      result = node(result, k, right);
    }
  }
  return true;
}

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
        throw syntax_error{"unterminated expression. Expected a unary-expression after " + token};
      result = node(result, k, right);
    }
  }
  return true;
}

bool parser::get_unary(node& result)
{
  std::string token{};
  kind k = get_token(token);
  if (k == eof)
    return false;
  if (k == '-') {
    if (not get_primary(result))
      throw syntax_error{"expected primary after unary " + token + ", got end of line"};
    result = node(k, result);
    return true;
  } else if (k == '+') {
    if (not get_primary(result))
      throw syntax_error{"expected primary after unary +, got end of line"};
    return true;
  } else {
    push_back(token, k);
    return get_primary(result);
  }
}

void parser::get_expr_list(node_list& result)
{
  result.clear();
  std::string token{};
  while (kind k = get_token(token)) {
    if (k == ')')
      return;
    push_back(token, k);
    node expr{};
    if (not get_expr(expr))
      throw syntax_error{"unexpected end of line in function argument"};
    result.push_back(expr);
    k = get_token(token);
    if (k == ')')
      return;
    else if (k != ',')
      throw syntax_error{"expected comma in argument list, got " + token};
  }
  throw syntax_error{"unexpected end of line in function argument list"};
}

bool parser::get_primary(node& result)
{
  std::string token{};
  kind k = get_token(token);
  if (k == eof)
    return false;

  if (k == '(') {
    // Parenthesized expression
    if (not get_expr(result))
      throw syntax_error{"expected expression, got end of line"};
    k = get_token(token);
    if (k != ')')
      throw syntax_error{"expected ')', got " + token};
    else
      return true;
  }

  if (k == number) {
    // Numeric literal
    if (not get_number(token, result))
      throw syntax_error{"Invalid numeric literal: " + token};
    return true;
  }

  if (k == identifier) {
    // Identifier: variable or function call
    std::string next{};
    k = get_token(next);
    if (k == '(') {
        // function call
      node_list arguments{};
      get_expr_list(arguments);
      result = node{std::move(token), std::move(arguments)};
    } else {
      static const node_list no_arguments{};
      // Variable reference or function call with no arguments
      push_back(next, k);
      result = node(std::move(token), no_arguments);
    }
    return true;
  }
  throw syntax_error("expected a primary, got " + token);
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
      while (p.get_statement(output)) {
        /* empty */
      }
    } catch(calc_error const& ex) {
      output << ex.what() << '\n';
    } catch(std::exception const& ex) {
      output << "exception: " << ex.what() << '\n';
    }
  }
}
