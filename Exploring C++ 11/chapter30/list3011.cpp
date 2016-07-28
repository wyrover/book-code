// Listing 30-11. Computing the Absolute Value of a Rational Number
rational absval(rational const& r)
{
  return rational{std::abs(r.numerator), r.denominator};
}
