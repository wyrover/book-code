// Listing 31-3. Testing the I/O Operators

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <sstream>

// Listing 29-3. Adding the reduce Member Function
/// Compute the greatest common divisor of two integers, using Euclid’s algorithm.
int gcd(int n, int m)
{
  n = std::abs(n);
  while (m != 0) {
    int tmp{n % m};
    n = m;
    m = tmp;
  }
  return n;
}

// Listing 30-1. Overloading the Equality Operator
/// Represent a rational number.
struct rational
{
  /// Construct a rational object, given a numerator and a denominator.
  /// Always reduce to normal form.
  /// @param num numerator
  /// @param den denominator
  /// @pre denominator > 0
  rational(int num, int den)
  : numerator{num}, denominator{den}
  {
    reduce();
  }

  /// Construct a rational object, given a numerator.
  /// No need to reduce to normal form because denominator is always 1.
  /// @param num numerator
  rational(int num)
  : numerator{num}, denominator{1}
  {}

  /// Assign a numerator and a denominator, then reduce to normal form.
  /// @param num numerator
  /// @param den denominator
  /// @pre denominator > 0
  void assign(int num, int den)
  {
    numerator = num;
    denominator = den;
    reduce();
  }

  /// Reduce the numerator and denominator by their GCD.
  void reduce()
  {
    assert(denominator != 0);
    if (denominator < 0)
    {
      denominator = -denominator;
      numerator = -numerator;
    }
    int div{gcd(numerator, denominator)};
    numerator = numerator / div;
    denominator = denominator / div;
  }

  int numerator;     ///< numerator gets the sign of the rational value
  int denominator;   ///< denominator is always positive
};

// Listing 30-2. Using inline for Trivial Functions
/// Compare two rational numbers for equality.
/// @pre @p a and @p b are reduced to normal form
bool operator==(rational const& a, rational const& b)
{
  return a.numerator == b.numerator and a.denominator == b.denominator;
}

/// Compare two rational numbers for inequality.
/// @pre @p a and @p b are reduced to normal form
inline bool operator!=(rational const& a, rational const& b)
{
  return not (a == b);
}

// Listing 30-3. Implementing the < Operator for rational
/// Compare two rational numbers for less-than.
bool operator<(rational const& a, rational const& b)
{
  return a.numerator * b.denominator < b.numerator * a.denominator;
}

/// Compare two rational numbers for less-than-or-equal.
inline bool operator<=(rational const& a, rational const& b)
{
  return not (b < a);
}

// Listing 30-4. Implementing the > and >= Operators in Terms of <
/// Compare two rational numbers for greater-than.
inline bool operator>(rational const& a, rational const& b)
{
  return b < a;
}

/// Compare two rational numbers for greater-than-or-equal.
inline bool operator>=(rational const& a, rational const& b)
{
  return not (b > a);
}

// Listing 30-6. Addition Operator for the rational Type
rational operator+(rational const& lhs, rational const& rhs)
{
  return rational{lhs.numerator * rhs.denominator + rhs.numerator * lhs.denominator,
                  lhs.denominator * rhs.denominator};
}

rational operator-(rational const& r)
{
  return rational{-r.numerator, r.denominator};
}

// Listing 30-7. Arithmetic Operators for the rational Type
rational operator-(rational const& lhs, rational const& rhs)
{
  return rational{lhs.numerator * rhs.denominator - rhs.numerator * lhs.denominator,
                  lhs.denominator * rhs.denominator};
}

rational operator*(rational const& lhs, rational const& rhs)
{
  return rational{lhs.numerator * rhs.numerator, lhs.denominator * rhs.denominator};
}

rational operator/(rational const& lhs, rational const& rhs)
{
  return rational{lhs.numerator * rhs.denominator, lhs.denominator * rhs.numerator};
}

std::istream& operator>>(std::istream& in, rational& rat)
{
  int n{0}, d{0};
  char sep{'\0'};
  if (not (in >> n >> sep))
    // Error reading the numerator or the separator character.
    in.setstate(std::cin.failbit);
  else if (sep != '/')
  {
    // Read numerator successfully, but it is not followed by /.
    // Push sep back into the input stream, so the next input operation
    // will read it.
    in.unget();
    rat.assign(n, 1);
  }
  else if (in >> d)
    // Successfully read numerator, separator, and denominator.
    rat.assign(n, d);
  else
    // Error reading denominator.
    in.setstate(std::cin.failbit);

  return in;
}

std::ostream& operator<<(std::ostream& out, rational const& rat)
{
  std::ostringstream tmp{};
  tmp << rat.numerator;
  if (rat.denominator != 1)
    tmp << '/' << rat.denominator;
  out << tmp.str();

  return out;
}

/// Tests for failbit only
bool iofailure(std::istream& in)
{
  return in.fail() and not in.bad();
}

int main()
{
  rational r{0};

  while (std::cin)
  {
    if (std::cin >> r)
      // Read succeeded, so no failure state is set in the stream.
      std::cout << r << '\n';
    else if (iofailure(std::cin))
    {
      // Only failbit is set, meaning invalid input. Clear the state, 
      // and then skip the rest of the input line.
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<int>::max(), '\n');
    }
  }

  if (std::cin.bad()) 
    std::cerr << "Unrecoverable input failure\n";
}
