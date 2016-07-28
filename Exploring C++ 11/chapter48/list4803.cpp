// Listing 48-3. Testing the absval Function Template
#include <iostream>
#include "rational.hpp"  // Listing 46-10


template<class T>
T absval(T x)
{
  if (x < 0)
    return -x;
  else
    return x;
}

int main()
{
  std::cout << absval(-42) << '\n';
  std::cout << absval(-4.2) << '\n';
  std::cout << absval(42) << '\n';
  std::cout << absval(4.2) << '\n';
  std::cout << absval(-42L) << '\n';
  std::cout << absval(rational{42, 5}) << '\n';
  std::cout << absval(rational{-42, 5}) << '\n';
  std::cout << absval(rational{42, -5}) << '\n';
}
