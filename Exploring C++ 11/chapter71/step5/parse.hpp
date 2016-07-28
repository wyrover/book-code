#ifndef PARSE_HPP_
#define PARSE_HPP_

#include <cstdlib>
#include <istream>
#include <locale>
#include <ostream>
#include <string>

#include "calc_error.hpp"
#include "node.hpp"

/** Parser class template.
 * The parser reads tokens from an input stream. A token can be a keyword,
 * numeric literal, identifier, or symbol (operator or punctuator).
 * Symbols can have multiple characters (e.g., :=).
 *
 * Because the recursive-descent parser can examine too many tokens from the
 * input stream, it keeps a push-back token. Once the parser knows it has
 * gone too far, it pushes back the most recently read token. The next call
 * to get_token() retrieves the pushed-back token.
 *
 * Only one push-back is available, which limits the complexity of the syntax.
 */
class parser
{
public:
  /// Token kind.
  /// Declare a name for each single-character token, to ensure the enumerated type
  /// can represent any operator or punctuator character.
  enum kind : int { eof, identifier, number, string,
    plus='+', minus='-', times='*', slash='/', lparen = '(', rparen=')', equal='=',
    comma=','};

    /** Constructor. Save the @p input stream.
   * @param input The input stream
   */
  parser(std::istream& input);

  /** Read one statement and store the parse tree in @p result.
   * If the statement is an assignment or function definition,
   * store the variable or function. If the statement is an expression,
   * print the result to @p output.
   * @verbatim
     STATEMENT ::= DEFINITION | QUIT | EXPR | SAVE | LOAD
     SAVE ::= "save" FILENAME
     LOAD ::= "load" FILENAME
     FILENAME ::= quoted-string
     @endverbatim
   * @param output The output stream.
   * @return true to continue or false to end the loop
   * @throw parse_error for various syntax and other errors
   */
  bool get_statement(std::ostream& output);

private:
  /** Convert a characer to a readable form.
   * @param c The character
   * @return A C++-style character literal that ensures @p c is readable.
   */
  std::string charify(char c);
  /** Parse a floating number.
   * @param token The token to parse
   * @param result Store the number here
   * @return true if @p token is a valid number or false for an error
   */
  bool get_number(std::string const& token, node& result);
  /** Parse an expression
   * @param result Store the result here
   * @return true to continue parsing or false to stop (end of file or error)
   */
  bool get_expr(node& result);
  /** Parse an addition expression
   * @verbatim
     ADD_EXPR ::= MUL_EXPR | ADD_EXPR + MUL_EXPR | ADD_EXPR - MUL_EXPR
     @endverbatim
   * @param result Store the result here
   * @return true to continue parsing or false to stop (end of file or error)
   */
  bool get_add_expr(node& result);
  /** Parse a multiplicative expression.
   * @verbatim
     MUL_EXPR ::= UNARY | MUL_EXPR + UNARY | MUL_EXPR - UNARY
     @endverbatim
   * @param result Store the result here
   * @return true to continue parsing or false to stop (end of file or error)
   */
  bool get_mul_expr(node& result);
  /** Parse a primary expression.
   * @verbatim
     PRIMARY ::= NUMBER | IDENTIFIER | '(' EXPR ')' | FUNCTION_CALL
     FUNCTION_CALL ::= IDENTIFIER '(' OPT_EXPR_LIST ')'
     OPT_EXPR_LIST ::= empty | EXPR_LIST
     EXPR_LIST ::= EXPR | EXPR_LIST ',' EXPR
     @endverbatim
   * @param result Store the result here
   * @return true to continue parsing or false to stop (end of file or error)
   */
  bool get_primary(node& result);
  /** Parse a unary expression.
   * @verbatim
     UNARY ::= '-' PRIMARY | '+' PRIMARY | PRIMARY
     @endverbatim
   * @param result Store the result here
   * @return true to continue parsing or false to stop (end of file or error)
   */
  bool get_unary(node& result);
  /** Parse a function or variable definition
   * A variable is just like a function that takes no parameters.
   * @verbatim
     DEFINITION ::= DEF IDENTIFIER OPT_PARAMETERS '=' EXPR
     OPT_PARAMETERS ::= emtpy | '(' OPT_IDENTIFIER_LIST ')'
     OPT_IDENTIFIER_LIST ::=  empty | IDENTIFIER_LIST
     IDENTIFIER_LIST ::= IDENTIFIER | IDENTIFIER_LIST ',' IDENTIFIER
     @endverbatim
   * @param[out] name Store the variable or function name here
   * @param[out] parameters Store the list of parameter names here
   * @param[out] definition Store the definition expression here
   */
  void get_definition(std::string& name, identifier_list& parameters, node& definition);
  /** Parse a token.
   * A token can be a keyword, a literal or a symbol.
   * @verbatim
     TOKEN ::= IDENTIFIER | NUMBER | SYMBOL | STRING
     IDENTIIFER ::= ALPHA (ALPHA | DIGIT)*
     NUMBER ::= DIGIT+ ('.' DIGITS+)? ('E' SIGN? DIGITS+)?
     SYMBOL ::= '+' | '-' | '*' | '/' | '%' | '(' | ')' | '=' | ','
     @endverbatim
   * @param token Store the text of the token here.
   * @return the token kind
   */
  kind get_token(std::string& token);
  /** Parse an identifer.
   * @param identifier Store the identifier here.
   * @pre first input character is alphabetic
   */
  void get_identifier(std::string& identifier);
  /** Parse a comma-separated expression list.
   * @param[out] result Store the result here
   */
  void get_expr_list(node_list& result);

  /** Parse a list of parameter names.
   * Names are identifiers, separated by commas.
   * The list can be empty.
   * This is a template so the container type is unimportant. Any output
   * iterator will do.
   * @param[out] output Store the identifiers here
   * @return a copy of @p output after storing all the identifiers
   */
  template<class OutputIterator>
  OutputIterator get_namelist(OutputIterator output);
  /** Interpret a backslash escape sequence.
   * The caller must have read the backslash already.
   * @param[out] str Write the string equivalent of the escape sequence at the end of this string
   */
  void get_escape(std::string& str);
  /** Parse a quoted string.
   * The caller passes the quote character in the @p delimiter argument.
   * @param[out] result Store the token here.
   * @param[in] delimiter The quote character (<tt>'</tt> or <tt>"</tt>)
   */
  void get_string(std::string& result, char delimiter);

  /** Push back a token.
   * The next call to get_token() will return the pushed-back token.
   * @param token The token to push back.
   * @param k The kind of token being pushed back
   */
  void push_back(std::string const& token, kind k);

  /** Return true if @p c is alphabetic.
   * Use the locale of the input stream.
   * @param c The character to test.
   */
  bool isalpha(char c) const { return ctype_.is(ctype_.alpha, c); }
  /** Return true if @p c is alphanumeric.
   * Use the locale of the input stream.
   * @param c The character to test.
   */
  bool isalnum(char c) const { return ctype_.is(ctype_.alnum, c); }
  /** Return true if @p c is a digit.
   * Use the locale of the input stream.
   * @param c The character to test.
   */
  bool isdigit(char c) const { return ctype_.is(ctype_.digit, c); }
  /** Return true if @p c is printable.
   * Use the locale of the input stream.
   * @param c The character to test.
   */
  bool isprint(char c) const { return ctype_.is(ctype_.print, c); }

  std::istream& input_;            ///< Share the input stream
  std::ctype<char> const& ctype_;  ///< Cache the ctype facet for checking character categories
  std::string token_;              ///< One token push-back
  kind kind_;                      ///< The kind of token that was pushed back.
};

/** Parse loop.
 * Read expressions from @p input and print results to @p output.
 * @param input The input stream.
 * @param output The output stream.
 */
void parse_loop(std::istream& input, std::ostream& output);

template<class OutputIterator>
OutputIterator parser::get_namelist(OutputIterator output)
{
  std::string token{};
  while (kind k = get_token(token)) {
    if (k == ')')
      return output;
    else if (k != identifier)
      throw syntax_error{"expected function parameter, got " + token};
    else {
      *output = token;
      ++output;

      k = get_token(token);
      if (k == ')')
        return output;
      if (k != ',')
        throw syntax_error{"expected comma in function paramter list, got " + token};
    }
  }
  throw syntax_error{"unexpected end of line in function parameter list"};
}


#endif
