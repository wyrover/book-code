// Listing 26-1. Floating-Point Numbers Do Not Always Behave As You Expect
#include <cassert>
int main()
{
  float a{0.03F};
  float b{10.0F};
  float c{0.3F};
  assert(a * b == c);
}
