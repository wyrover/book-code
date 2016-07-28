// Listing 12-2. Automatic Type Conversion to bool
#include <iostream>

int main()
{
  if (true)        std::cout << "true\n";
  if (false)       std::cout << "false\n";
  if (42)          std::cout << "42\n";
  if (0)           std::cout << "0\n";
  if (42.4242)     std::cout << "42.4242\n";
  if (0.0)         std::cout << "0.0\n";
  if (-0.0)        std::cout << "-0.0\n";
  if (-1)          std::cout << "-1\n";
  if ('\0')        std::cout << "'\\0'\n";
  if ('\1')        std::cout << "'\\1'\n";
  if ("1")         std::cout << "\"1\"\n";
  if ("false")     std::cout << "\"false\"\n";
  if (std::cout)   std::cout << "std::cout\n";
  if (std::cin)    std::cout << "std::cin\n";


}
