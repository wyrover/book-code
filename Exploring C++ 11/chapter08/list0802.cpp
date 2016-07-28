// Listing 8-2. Printing a Table of Powers the Right Way 
#include <iomanip>
#include <iostream>

int main()
{
  std::cout << " N   N^2    N^3\n";
  for (int i{1}; i != 21; ++i)
    std::cout << std::setw(2) << i
              << std::setw(6) << i*i
              << std::setw(7) << i*i*i
              << '\n';
}
