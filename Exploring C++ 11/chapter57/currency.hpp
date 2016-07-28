/** @file currency.hpp
 * Implement a currency type.
 */

/** @mainpage Project 3: Currency Type
 * It’s time for another project. You’re going to continue building on the
 * @c fixed type from Project 2 and incorporate what you’ve learned about
 * locales and I/O. Your task this time is to write a @c currency type.
 *
 * The value is stored as a fixed-point value.
 * I/O is formatted using the @c get_money and @c put_money manipulators.
 *
 * Make sure you can add two currency amounts to get a currency value,
 * subtract two currency amounts to get currency,
 * multiply and divide currency by an integer or rational value
 * to get a currency result, and divide two currency values
 * to get a rational result.
 *
 * As with any project, start small and add functionality as you go.
 * For example, start with the basic data representation,
 * then add I/O operators. Add arithmetic operators one at a time.
 * Write each test function before you implement the feature.
*/

#ifndef CURRENCY_HPP_
#define CURRENCY_HPP_

#include <iomanip>
#include <istream>
#include <locale>
#include <ostream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "ioflags.hpp"
#include "fixed.hpp"
#include "rational.hpp"

/** Class to represent a currency value in the global locale. */
template<class T=long, int N=2>
class currency
{
public:
  typedef T int_type;                    ///< Storage type
  typedef fixed<T,N> value_type;         ///< Type of the actual value

  /// Default constructor initializes the value to zero.
  constexpr currency() : value_() {}
  /// Initialize the value to @p integer
  /// @param integer The integer initial value; the fractional part is zero.
  constexpr currency(T integer) : value_(integer) {}
  /// Initialize the value.
  /// The interpretation of the fractional part depends on @p N.
  /// For example, if @p N is 2, a @p fraction of 9 represents 0.09,
  /// buf if @p N is 5, @p fraction of 9 means 0.0009.
  /// @param integer The integer part of the initial value
  /// @param fraction The fractional part of the initial value
  currency(T integer, T fraction) : value_(integer, fraction) {}
  /// Initialize from a floating point number.
  /// @param value the initial value
  currency(double value) : value_(value) {}

  /// Copy a value that uses a different precision.
  template<class U, int M>
  currency(currency<U, M> const& rhs): value_(rhs.value()) {}

  /// Assign a value that uses a different precision.
  template<class U, int M>
  currency& operator=(currency<U, M> rhs)
  {
    value_ = rhs.value();
    return *this;
  }

  /// Convert to a string.
  /// @returns a string representation of the value, e.g., "$123.04"
  std::string as_string() const;
  /// Overwrite this value with the value read from the stream.
  /// The value in the stream must have the correct number of digits.
  /// If the showbase flag is set, the currency symbol must be present.
  /// @param strm Input stream
  /// @return true if the read is success and @c *this has been modified,
  /// or false if the read fails. Check @p strm for details.
  template<class Char, class Traits>
  bool read(std::basic_istream<Char, Traits>& strm);

  /// Convert the value to a different numeric type.
  /// Typically, the other type is a floating-point type.
  template<class U>
  /// Convert to some other type, especially floating point.
  U convert() const { return value().template convert<U>(); }

  /// Round off to the nearest integer, using banker's rounding.
  int_type round() const { return value().round(); }

  /// Return the integer part (which is the same as trunc()).
  int_type integer()  const { return value().integer(); }
  /// Return the fractional part, to @p M places.
  template<int M>
  int_type fraction() const { return value().template fraction<M>(); }
  /// Return the fractional part.
  int_type fraction() const { return value().fraction(); }

  /// Addition operator.
  /// @param c the value to add
  /// @return @c *this
  currency& operator+=(currency c);
  /// Subtraction operator.
  /// @param c the value to subtract
  /// @return @c *this
  currency& operator-=(currency c);
  /// Multiplication operator.
  /// @param m the value to multiply
  /// @return @c *this
  currency& operator*=(value_type m);
  /// Multiplication operator.
  /// @param m the value to multiply
  /// @return @c *this
  currency& operator*=(int_type m);
  /// Division operator.
  /// @param m the divisor
  /// @return @c *this
  currency& operator/=(value_type m);
  /// Division operator.
  /// @param m the divisor
  /// @return @c *this
  currency& operator/=(int_type m);

  /// Negate this value.
  void negate();

  /// Pre-increment operator.
  currency& operator++();
  /// Post-increment operator.
  currency operator++(int);
  /// Pre-decrement operator.
  currency& operator--();
  /// Post-decrement operator.
  currency operator--(int);

  /// Return the internal value.
  value_type value()    const { return value_; }

private:
  value_type value_;
};

template<class T, int N>
std::string currency<T,N>::as_string()
const
{
  std::ostringstream digits;
  digits.imbue(std::locale::classic());
  digits << integer() << std::setw(value_type::places) << std::setfill('0') << fraction();

  std::ostringstream out;
  std::money_put<char> const& put(std::use_facet<std::money_put<char> >(std::locale()));
  put.put(std::ostreambuf_iterator<char>(out), false, out, '0', digits.str());
  return out.str();
}

template<class T, int N>
currency<T,N>& currency<T,N>::operator+=(currency f)
{
  value_ += f.value();
  return *this;
}

template<class T, int N>
currency<T,N>& currency<T,N>::operator-=(currency f)
{
  value_ -= f.value();
  return *this;
}

template<class T, int N>
currency<T,N>& currency<T,N>::operator*=(value_type i)
{
  value_ *= i;
  return *this;
}
template<class T, int N>
currency<T,N>& currency<T,N>::operator*=(int_type i)
{
  value_ *= i;
  return *this;
}

template<class T, int N>
currency<T,N>& currency<T,N>::operator/=(value_type i)
{
  value_ /= i;
  return *this;
}
template<class T, int N>
currency<T,N>& currency<T,N>::operator/=(int_type i)
{
  value_ /= i;
  return *this;
}

template<class T, int N>
void currency<T,N>::negate()
{
  value_ = -value_;
}

template<class T, int N>
currency<T,N>& currency<T,N>::operator++()
{
  ++value_;
  return *this;
}

template<class T, int N>
currency<T,N> currency<T,N>::operator++(int)
{
  currency result(*this);
  ++value_;
  return result;
}

template<class T, int N>
currency<T,N>& currency<T,N>::operator--()
{
  --value_;
  return *this;
}

template<class T, int N>
currency<T,N> currency<T,N>::operator--(int)
{
  currency result(*this);
  --value_;
  return result;
}

template<class T, int N>
template<class CharT, class Traits>
bool currency<T,N>::read(std::basic_istream<CharT,Traits>& strm)
{
  ioflags flags(strm);
  typename std::basic_istream<CharT, Traits>::sentry sentry(strm, false);
  if (not sentry)
    return false;

  std::ios_base::iostate error(std::ios_base::goodbit);
  std::string digits;
  std::money_get<CharT> const& get(
      std::use_facet<std::money_get<CharT> >(strm.getloc()));
  get.get(std::istreambuf_iterator<CharT>(strm), std::istreambuf_iterator<CharT>(),
          false, strm, error, digits);

  if ((error & std::ios_base::failbit) != 0)
    return false;

  std::moneypunct<CharT> const& punct(
      std::use_facet<std::moneypunct<CharT> >(strm.getloc()));

  // Set fraction to the rightmost frac_digits() characters of digits.
  std::string fraction(digits.substr(digits.size() - punct.frac_digits(), punct.frac_digits()));
  // Set integer to the remainder of digits.
  std::string integer(digits.substr(0, digits.size() - punct.frac_digits()));

  std::istringstream fixed_stream(integer + "." + fraction);
  return value_.read(fixed_stream);
}

/// Read a currency value
/// @param strm The input stream
/// @param[out] c Store the value here
template<class T, int N, class Char, class Traits>
std::basic_istream<Char, Traits>& operator>>(std::basic_istream<Char, Traits>& strm, currency<T,N>& c)
{
  if (not c.read(strm))
    strm.setstate(strm.failbit);
  return strm;
}

/// Write a currency value
/// @param strm The output stream
/// @param c The value to write
template<class T, int N, class Char, class Traits>
std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& strm, currency<T,N> c)
{
  typename std::basic_ostream<Char, Traits>::sentry sentry(strm);
  strm << c.as_string();
  return strm;
}

/// Negate a currency value
template<class T, int N>
currency<T,N> operator-(currency<T,N> a)
{
  a.negate();
  return a;
}

/// Add currency values
/// @param a The left-hand operand
/// @param b The right-hand operand
template<class T, int N>
currency<T,N> operator+(currency<T,N> a, currency<T,N> b)
{
  a += b;
  return a;
}

/// Subtract currency values
/// @param a The left-hand operand
/// @param b The right-hand operand
template<class T, int N>
currency<T,N> operator-(currency<T,N> a, currency<T,N> b)
{
  a -= b;
  return a;
}

/// Multiply currency value and an integer
/// @param a The left-hand operand
/// @param b The right-hand operand
template<class T, int N>
currency<T,N> operator*(currency<T,N> a, T b)
{
  a *= b;
  return a;
}

/// Multiply currency value and an integer
/// @param a The left-hand operand
/// @param b The right-hand operand
template<class T, int N>
currency<T,N> operator*(T a, currency<T,N> b)
{
  b *= a;
  return b;
}

/// Divide currency value by an integer
/// @param a The left-hand operand
/// @param b The right-hand operand
template<class T, int N>
currency<T,N> operator/(currency<T,N> a, T b)
{
  a /= b;
  return a;
}

/// Divide currency values to yield a rational result.
/// @param n the numerator
/// @param d the denominator
template<class T, int N>
rational<T> operator/(currency<T,N> n, currency<T,N> d)
{
  // Extract the underlying value of the fixed values. No adjustment
  // to scaling is needed because the numerator and denominator are
  // both scaled to the same amount.
  return rational<T>(n.value().value(), d.value().value());
}

/// Compare currency values for equality by comparing the underlying values.
/// @param a The left-hand operand
/// @param b The right-hand operand
template<class T, int N>
bool operator==(currency<T,N> a, currency<T,N> b)
{
  return a.value() == b.value();
}
/// Compare currency value and an integer for equality
/// @param a The left-hand operand
/// @param b The right-hand operand
template<class T, int N>
bool operator==(currency<T,N> a, T b)
{
  return a.value() == b;
}
/// Compare currency value and an integer for equality
/// @param a The left-hand operand
/// @param b The right-hand operand
template<class T, int N>
bool operator==(T a, currency<T,N> b)
{
  return a == b.value();
}

/// Compare currency values for inequality.
/// @param a The left-hand operand
/// @param b The right-hand operand
template<class T, int N>
inline bool operator!=(currency<T,N> a, currency<T,N> b)
{
  return not (a == b);
}
/// Compare currency value and an integer for inequality
/// @param a The left-hand operand
/// @param b The right-hand operand
template<class T, int N>
inline bool operator!=(currency<T,N> a, T b)
{
  return not (a == b);
}
/// Compare currency value and an integer for inequality
/// @param a The left-hand operand
/// @param b The right-hand operand
template<class T, int N>
inline bool operator!=(T a, currency<T,N> b)
{
  return not (a == b);
}

/// Compare currency values for less-than by comparing the underlying values.
/// @param a The left-hand operand
/// @param b The right-hand operand
template<class T, int N>
bool operator<(currency<T,N> a, currency<T,N> b)
{
  return a.value() < b.value();
}
/// Compare a currency value and an integer for less-than.
/// @param a The left-hand operand
/// @param b The right-hand operand
template<class T, int N>
bool operator<(currency<T,N> a, T b)
{
  return a.value() < b;
}
/// Compare a currency value and an integer for less-than.
/// @param a The left-hand operand
/// @param b The right-hand operand
template<class T, int N>
bool operator<(T a, currency<T,N> b)
{
  return a < b.value();
}

/// Compare currency values for greater-than.
/// @param a The left-hand operand
/// @param b The right-hand operand
template<class T, int N>
inline bool operator>(currency<T,N> a, currency<T,N> b)
{
  return b < a;
}
/// Compare a currency value and an integer for greater-than.
/// @param a The left-hand operand
/// @param b The right-hand operand
template<class T, int N>
inline bool operator>(currency<T,N> a, T b)
{
  return b < a;
}
/// Compare a currency value and an integer for greater-than.
/// @param a The left-hand operand
/// @param b The right-hand operand
template<class T, int N>
inline bool operator>(T a, currency<T,N> b)
{
  return b < a;
}

/// Compare currency values for less-than-or-equal.
/// @param a The left-hand operand
/// @param b The right-hand operand
template<class T, int N>
inline bool operator<=(currency<T,N> a, currency<T,N> b)
{
  return not (b < a);
}
/// Compare a currency value and an integer for less-than-or-equal.
/// @param a The left-hand operand
/// @param b The right-hand operand
template<class T, int N>
inline bool operator<=(currency<T,N> a, T b)
{
  return not (b < a);
}
/// Compare a currency value and an integer for less-than-or-equal.
/// @param a The left-hand operand
/// @param b The right-hand operand
template<class T, int N>
inline bool operator<=(T a, currency<T,N> b)
{
  return not (b < a);
}

/// Compare currency values for greater-than-or-equal.
/// @param a The left-hand operand
/// @param b The right-hand operand
template<class T, int N>
inline bool operator>=(currency<T,N> a, currency<T,N> b)
{
  return not (a < b);
}
/// Compare a currency value and an integer for greater-than-or-equal.
/// @param a The left-hand operand
/// @param b The right-hand operand
template<class T, int N>
inline bool operator>=(currency<T,N> a, T b)
{
  return not (a < b);
}
/// Compare a currency value and an integer for greater-than-or-equal.
/// @param a The left-hand operand
/// @param b The right-hand operand
template<class T, int N>
inline bool operator>=(T a, currency<T,N> b)
{
  return not (a < b);
}

#endif
