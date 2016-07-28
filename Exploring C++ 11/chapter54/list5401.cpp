// Listing 54-1. Reading and Writing Currency Using the Money I/O Manipulators
#include <iomanip>
#include <iostream>
#include <iterator>
#include <locale>
#include <string>

int main()
{
  std::locale native{""};
  std::cin.imbue(native);
  std::cout.imbue(native);

  std::cin >> std::noshowbase;  // currency symbol is optional for input
  std::cout << std::showbase;   // always write the currency symbol for output

  std::string digits;
  while (std::cin >> std::get_money(digits))
  {
    std::cout << std::put_money(digits) << '\n';
  }
  if (not std::cin.eof())
    std::cout << "Invalid input.\n";
}
