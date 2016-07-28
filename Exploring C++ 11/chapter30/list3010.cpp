// Listing 30-10. Optimizing Operators for a Specific Operand Type
rational operator*(rational const& rat, int mult)
{
  return rational{rat.numerator * mult, rat.denominator};
}

inline rational operator*(int mult, rational const& rat)
{
  return rat * mult;
}

