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
