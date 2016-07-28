#ifndef NUMBER_HPP_
#define NUMBER_HPP_

#include <memory>
#include <ostream>
#include <string>

#include "rational.hpp"

/// Forward declaration of the implementation base class.
class number_impl;

/** Number class represents a numeric value.
 * Numbers of different types are supported, as well as conversions between types.
 */
class number
{
public:
  /** Construct a void value.
   * The calculator does not currently use void, but I can't think of a better
   * value for a default number constructor.
   */
  number();
  /** Construct an @c int value.
   * For the sake of simplicitly, all integers are stored as @c long @c long values.
   * This overloaded operator exists so simple uses of, say, number(0),
   * are not ambiguous.
   * @param x The value of the number
   */
  number(int x);
  /** Construct a @c long value.
   * For the sake of simplicitly, all integers are stored as @c long @c long values.
   * @param x The value of the number
   */
  number(long x);
  /** Construct a @c long @c long value.
   * For the sake of simplicitly, all integers are stored as @c long @c long values.
   * @param x The value of the number
   */
  number(long long x);
  /** Construct a floating-point value.
   * For the sake of simplicitly, all floating-point numbers
   * are stored as @c double values.
   * @param x The value of the number
   */
  number(double x);
  /** Construct a rational number.
   * @param x The value of the number
   */
  number(rational<long long> const& x);

  number(number const&) = default;
  number(number&&) = default;
  number& operator=(number const&) = default;
  number& operator=(number&&) = default;
  ~number() = default;

  /** Construct a number by reading from a library file. */
  number(std::istream& stream);

  /** Save a number to a library file.
   * @param stream the library stream
   */
  void save(std::ostream& stream)  const;

  /** Test for equality.
   * Coerce @c this and @p b to the same type, then compare.
   * @param b the number to compare
   * @return true if @c *this is numerically equal to @p b
   */
  bool equals(number b);

  /** Test for less-than.
   * Coerce @c this and @p b to the same type, then compare.
   * @param b the number to compare
   * @return true if @c *this is numerically less than @p b
   */
  bool less(number b);

  /** Coerce @c this and @p rhs to the same arithmetic type.
   * Promote the "smaller" number to the "larger" type.
   * - number_long promotes to number_double
   * - everything promotes to void
   * @param[in,out] rhs the right-hand-side operand
   */
  void coerce(number& rhs);

  /** Convert this number to a human-readable string.
   */
  std::string to_string() const;

  /** Print a number to a stream.
   * @param stream The output stream
   */
  void print(std::ostream& stream);

  /** Addition operator.
   * Coerce @c this and @p rhs to the same type, then add @p rhs and @c this.
   * @param rhs The right-hand-side operand
   * @return @c *this + @p rhs
   */
  number operator+(number rhs);
  /** Subtraction operator.
   * Coerce @c this and @p rhs to the same type, then subtract @p rhs from @c this.
   * @param rhs The right-hand-side operand
   * @return @c *this - @p rhs
   */
  number operator-(number rhs);
  /** Multiplication operator.
   * Coerce @c this and @p rhs to the same type, then multiply @c this by @p rhs.
   * @param rhs The right-hand-side operand
   * @return @c *this * @p rhs
   */
  number operator*(number rhs);
  /** Division operator.
   * Coerce @c this and @p rhs to the same type, then divide @c this by @p rhs.
   * @param rhs The right-hand-side operand
   * @return @c *this / @p rhs
   * @throw calc_error if @p rhs == 0.
   */
  number operator/(number rhs);

private:
  /** Private constructor to create a number from a number_impl.
   * @param ptr Pointer to a number_impl.
   */
  number(std::shared_ptr<number_impl> ptr);

  std::shared_ptr<number_impl> pimpl_;
};

/** Compare two number objects for numeric equality.
 * The objects are coerced to a common type before comparing.
 * @param a The left-hand operand
 * @param b The right-hand operand
 * @return true if a == b (watch out for floating-point comparisons).
 */
inline bool operator==(number a, number b)
{
  return a.equals(b);
}

/** Compare two number objects for numeric inequality.
 * The objects are coerced to a common type before comparing.
 * @param a The left-hand operand
 * @param b The right-hand operand
 * @return true if a != b (watch out for floating-point comparisons).
 */
inline bool operator!=(number a, number b)
{
  return not (a == b);
}

/** Compare two number objects for numeric less-than.
 * The objects are coerced to a common type before comparing.
 * @param a The left-hand operand
 * @param b The right-hand operand
 * @return true if a \< b (watch out for floating-point comparisons).
 */
inline bool operator<(number a, number b)
{
  return a.less(b);
}

/** Compare two number objects for numeric less-than-or-equal.
 * The objects are coerced to a common type before comparing.
 * @param a The left-hand operand
 * @param b The right-hand operand
 * @return true if a \<= b (watch out for floating-point comparisons).
 */
inline bool operator<=(number a, number b)
{
  return not(b < a);
}

/** Compare two number objects for numeric greater-than.
 * The objects are coerced to a common type before comparing.
 * @param a The left-hand operand
 * @param b The right-hand operand
 * @return true if a > b (watch out for floating-point comparisons).
 */
inline bool operator>(number a, number b)
{
  return b < a;
}

/** Compare two number objects for numeric greater-than-or-equal.
 * The objects are coerced to a common type before comparing.
 * @param a The left-hand operand
 * @param b The right-hand operand
 * @return true if a >= b (watch out for floating-point comparisons).
 */
inline bool operator>=(number a, number b)
{
  return not (a < b);
}

/** Negate a number.
 * @param n the number
 * @return -@p n
 */
inline number operator-(number n)
{
  return number{0} - n;
}

/** Print a number
 * @param stream The output stream
 * @param n The number to print
 */
inline std::ostream& operator<<(std::ostream& stream, number n)
{
  n.print(stream);
  return stream;
}

#endif
