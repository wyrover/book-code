#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <stdexcept>
#include <string>

#include "fixed.hpp"
#include "ioflags.hpp"

// Construct a fixed value from an integer part and a fraction part
fixed::fixed(value_type integer, value_type fraction)
{
  if (fraction < 0)
    throw std::invalid_argument("negative fraction not allowed");
  fraction = reduce(fraction);
  if (integer < 0)
    value_ = integer * places10 - fraction;
  else
    value_ = integer * places10 + fraction;
}

// Construct a fixed value from an integer part with no fraction
fixed::fixed(value_type integer)
: value_{integer * places10}
{}

// Get the fraction part
fixed::value_type fixed::fraction()
const
{
  return std::abs(value()) % places10;
}

/// Reduce the fractional part to the range [0, places10).
/// Imagine frac has the format F(G(XY*)?)?.
/// The resulting value is FH, where H == 0 if G is absent,
/// or H == G+1 if X==5 and Y* == 0* and G is odd, or
/// H == G+1 if X>5 or X==5 and Y*>0*, else H == G.
/// In other words, check that frac ends with only zero digits,
/// then a 5, then two more digits (searching from least-significant
/// to most-significant). If so, implement banker's rounding.
/// Otherwise, round GXY* to the nearest value (G+1 or G).
fixed::value_type fixed::reduce(value_type frac)
{
  // First scan for zero digits on the right.
  value_type f{frac};
  while (f >= places10*10 and f % 10 == 0)
  {
    f /= 10;
  }

  if (f >= places10*10)
  {
    int x{0};
    // Loop ended because a non-zero digit was seen so Y* > 0.
    // Discard the remaining digits, but keep track of the last
    // digit to be processed (X).
    while (f >= places10)
    {
      x = f % 10;
      f /= 10;
    }
    // Round up if the last digit (X) is 5 or more
    if (x >= 5)
      ++f;
    return f;
  }
  // Else all digits so far are zero. Check how many digits there were,
  // that is, check whether G, and X at least are present.
  else if (f >= places10)
  {
    // Yes, G and X are present. If X == 5, implement banker's rounding.
    // Otherwise, round to nearest.
    int x{f % 10};
    f /= 10;
    assert(f < places10);
    if (x == 5)
    {
      // Yes, so implement banker's rounding.
      if (f % 2 != 0)
        ++f;
      return f;
    }
    else if (x < 5)
    {
      // Round down.
      return f;
    }
    else
    {
      // Round up.
      return f + 1;
    }
  }
  // Not enough digits, so nothing to round.
  assert(frac < places10);
  return frac;
}

// Round off to nearest integer.
fixed::value_type fixed::round()
const
{
  const value_type frac{fraction()};
  int adjust{value() < 0 ? -1 : +1};
  if (frac > places10/2)
    return integer()+adjust;
  else if (frac < places10/2)
    return integer();
  else if (integer() % 2 == 0)
    return integer();
  else
    return integer()+adjust;
}

// Convert to a string using fixed-point notation.
std::string fixed::as_string()
const
{
  std::ostringstream out{};
  out << integer() << '.'
      << std::setfill('0') << std::setw(places) << fraction();
  return out.str();
}

fixed& fixed::operator+=(fixed f)
{
  value_ += f.value();
  return *this;
}

fixed& fixed::operator-=(fixed f)
{
  value_ -= f.value();
  return *this;
}

fixed& fixed::operator*=(fixed f)
{
  value_ = (value_ * f.value()) / places10;
  return *this;
}

fixed& fixed::operator/=(fixed f)
{
  value_ = (value_ * places10) / f.value();
  return *this;
}

void fixed::negate()
{
  value_ = -value_;
}

fixed& fixed::operator++()
{
  value_ += places10;
  return *this;
}

fixed fixed::operator++(int)
{
  fixed result{*this};
  ++*this;
  return result;
}

fixed& fixed::operator--()
{
  value_ -= places10;
  return *this;
}

fixed fixed::operator--(int)
{
  fixed result{*this};
  --*this;
  return result;
}

fixed operator-(fixed a)
{
  a.negate();
  return a;
}

bool fixed::read(std::istream& strm)
{
  ioflags flags(strm);

  value_type integer{};
  char decimal{};
  if (not (strm >> integer))
    return false;
  strm.unsetf(std::ios_base::skipws);
  if (not (strm >> decimal) or decimal != '.')
  {
    // Just an integer is fine. Push back the non-decimal character,
    // if there is one, and reset the stream flags to show that
    // reading the fixed value succeeded.
    strm.unget();
    strm.clear(strm.rdstate() & ~strm.failbit);
    value_ = integer * places10;
    return true;
  }
  else
  {
    value_type fraction{0};
    char c{};
    int p{0};
    // Read one extra place for round-off.
    for (;
         p != places+1 and strm >> c and std::isdigit(c, strm.getloc());
         ++p)
    {
        fraction = fraction * 10 + (c - '0');
    }
    // Pad out to the requisite number of decimal places.
    for (; p < places; ++p)
      fraction = fraction * 10;
    // If the loop terminated because the maximum number of decimal
    // places were read, keep reading the stream to discard excees digits.
    while (strm and std::isdigit(c, strm.getloc()))
      strm >> c;
    // Push back the last, non-digit character read from the stream.
    // If the stream reached EOF, unget() is harmless.
    strm.unget();
    // Clear failbit because even if reading a character or whatever
    // failed, reading the fixed value did not.
    strm.clear(strm.rdstate() & ~strm.failbit);
    fraction = reduce(fraction);
    if (integer < 0)
      value_ = integer * places10 - fraction;
    else
      value_ = integer * places10 + fraction;
  }
  return true;
}

std::istream& operator>>(std::istream& strm, fixed& f)
{
  if (not f.read(strm))
    strm.setstate(strm.failbit);
  return strm;
}

std::ostream& operator<<(std::ostream& strm, fixed f)
{
  strm << f.as_string();
  return strm;
}

fixed operator+(fixed a, fixed b)
{
  a += b;
  return a;
}

fixed operator-(fixed a, fixed b)
{
  a -= b;
  return a;
}

fixed operator*(fixed a, fixed b)
{
  a *= b;
  return a;
}

fixed operator/(fixed a, fixed b)
{
  a /= b;
  return a;
}

bool operator==(fixed a, fixed b)
{
  return a.value() == b.value();
}

bool operator!=(fixed a, fixed b)
{
  return not (a == b);
}

bool operator<(fixed a, fixed b)
{
  return a.value() < b.value();
}

bool operator>(fixed a, fixed b)
{
  return b < a;
}

bool operator<=(fixed a, fixed b)
{
  return not (b < a);
}

bool operator>=(fixed a, fixed b)
{
  return not (a < b);
}
