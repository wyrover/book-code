// Listing 30-9. Constructing a Rational Number from a Floating-Point Argument
struct rational
{
  rational(int num, int den)
  : numerator{num}, denominator{den}
  {
    reduce();
  }

  rational(double r)
  : rational{static_cast<int>(r * 10000), 10000}
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

  int numerator;
  int denominator;
};
