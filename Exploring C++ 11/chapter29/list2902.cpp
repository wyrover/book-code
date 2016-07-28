// Listing 29-2. Using a Custom Type and Its Members
#include <iostream>

/// Represent a rational number.
struct rational
{
  int numerator;     ///< numerator gets the sign of the rational value
  int denominator;   ///< denominator is always positive
};

int main()
{
  rational pi{};
  pi.numerator = 355;
  pi.denominator = 113;
  std::cout << "pi is approximately " << pi.numerator << "/" << pi.denominator << '\n';
}
