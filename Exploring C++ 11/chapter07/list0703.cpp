// Listing 7-3. Using and Reusing a Loop Control Variable Name
#include <iostream>

int main()
{
  std::cout << '+';
  for (int i{0}; i != 20; i = i + 1)
    std::cout << '-';
  std::cout << "+\n|";

  for (int i{0}; i != 3; i = i + 1)
    std::cout << ' ';
  std::cout << "Hello, reader!";

  for (int i{0}; i != 3; i = i + 1)
    std::cout << ' ';
  std::cout << "|\n+";

  for (int i{0}; i != 20; i = i + 1)
    std::cout << '-';
  std::cout << "+\n";
}
