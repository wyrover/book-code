// Listing 32-1. First Version of the Assignment Operator
struct rational
{
  rational(int num, int den)
  : numerator{num}, denominator{den}
  {
    reduce();
  }

  rational& operator=(rational const& rhs)
  {
    numerator = rhs.numerator;
    denominator = rhs.denominator;
    reduce();
    return *this;
  }
  int numerator;
  int denominator;
};
