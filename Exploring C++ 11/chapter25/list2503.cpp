// Listing 25-3. Using Overloading to Distinguish Types of Integer Literals
#include <iostream>
#include <locale>

void print(int value)
{
  std::cout << "int=" << value << '\n';
}

void print(long value)
{
  std::cout << "long=" << value << '\n';
}

void print(long long value)
{
  std::cout << "long long=" << value << '\n';
}

int main()
{
  std::cout.imbue(std::locale{""});
  print(0);
  print(0L);
  print(32768);
  print(-32768);
  print(2147483647);
  print(-2147483647);
  print(2147483648);

  print(9223372036854775807);
  print(-9223372036854775807);
}
