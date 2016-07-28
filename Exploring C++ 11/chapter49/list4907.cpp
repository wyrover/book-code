// Listing 49-7. Trying to Mix rational Base Types
#include "rational.hpp"

int main()
{
  rational<int> little{};
  rational<long> big{};
  big = little;
}
