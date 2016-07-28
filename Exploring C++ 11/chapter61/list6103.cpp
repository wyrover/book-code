// Listing 61-3. Demonstrating Issues with Exceptions and Dynamic Memory
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

int* read(std::istream& in)
{
  int value{};
  if (in >> value)
    return new int{value};
  else
    return nullptr;
}

int divide(int x, int y)
{
  if (y == 0)
    throw std::runtime_error{"integer divide by zero"};
  else if (x < y)
    throw std::underflow_error{"result is less than 1"};
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
      if (int* x{read(input)})
      {
        if (int* y{read(input)})
        {
          std::cout << divide(*x, *y) << '\n';
          delete y;
        }
        delete x;
      }
    } catch (std::exception const& ex) {
      std::cout << ex.what() << '\n';
    }
}
