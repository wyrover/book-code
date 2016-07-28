#ifndef PARSE_HPP_
#define PARSE_HPP_

#include <istream>
#include <locale>
#include <ostream>
#include <stdexcept>
#include <string>

class node;

/** Exception class for parser errors, usually syntax errors. */
class parse_error : public std::runtime_error {
public:
  parse_error(std::string const& msg) : runtime_error{msg} {}
};

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
  enum kind : int { eof, identifier, number,
    plus='+', minus='-', times='*', slash='/', lparen = '(', rparen=')', equal='=' };

  /** Constructor. Save the @p input stream.
   * @param input The input stream
   */
  parser(std::istream& input);

  /** Read one expression and store the parse tree in @p result.
   * @param result Where to store the parse tree.
   * @return true to continue or false to end the loop
   * @throw parse_error for various syntax and other errors
   */
  bool get_expr(node& result);

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
     PRIMARY ::= NUMBER | IDENTIFIER | '(' EXPR ')'
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
  /** Parse a token.
   * A token can be a keyword, a literal or a symbol.
   * @verbatim
     TOKEN ::= IDENTIFIER | NUMBER | SYMBOL
     IDENTIIFER ::= ALPHA (ALPHA | DIGIT)*
     NUMBER ::= DIGIT+ ('.' DIGITS+)? ('E' SIGN? DIGITS+)?
     SYMBOL ::= '+' | '-' | '*' | '/' | '%' | '(' | ')' | '='
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

  /** Push back a token.
   * The next call to get_token() will return the pushed-back token.
   * @param token The token to push back.
   * @param k The kind of token being pushed back
   */
  void push_back(std::string const& token, kind k) { token_ = token; kind_ = k; }

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

#endif
