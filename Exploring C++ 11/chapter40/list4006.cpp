// Listing 40-6. Calling generate_id to Demonstrate Static Variables
#include <iostream>

int generate_id()
{
  static int counter{0};
  ++counter;
  return counter;
}

int main()
{
  for (int i{0}; i != 10; ++i)
    std::cout << generate_id() << '\n';
}
