// Listing 61-5. Fixing Memory Leaks
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

std::unique_ptr<int> read(std::istream& in)
{
  int value;
  if (in >> value)
    return std::unique_ptr<int>{new int{value}};
  else
    return std::unique_ptr<int>{};
}

int divide(int x, int y)
{
  if (y == 0)
    throw std::runtime_error("integer divide by zero");
  else if (x < y)
    throw std::underflow_error("result is less than 1");
  else
    return x / y;
}

int main()
{
  std::cout << "Enter pairs of numbers, and I will divide them.\n";
  std::string line{};
  while(std::getline(std::cin, line))
    try
    {
      std::istringstream input{line};
      if (std::unique_ptr<int> x{read(input)})
      {
        if (std::unique_ptr<int> y{read(input)})
          std::cout << divide(*x, *y) << '\n';
      }
    } catch (std::exception const& ex) {
      std::cout << ex.what() << '\n';
    }
}
