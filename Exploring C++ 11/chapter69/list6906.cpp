// Listing 69-6. Default Arguments
#include <iostream>

int add(int x = 0, int y = 0)
{
  return x + y;
}

int main()
{
  std::cout << add() << '\n';
  std::cout << add(5) << '\n';
  std::cout << add(32, add(4, add(6))) << '\n';
}
