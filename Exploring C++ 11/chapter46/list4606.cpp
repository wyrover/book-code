// Listing 46-6. Reimplementing Multiplication in Terms of an Assignment Operator
rational operator*(rational const& lhs, rational const& rhs)
{
  rational result{lhs};
  result *= rhs;
  return result;
}
