// Listing 25-4. Using Type Casts
#include <iostream>
#include <locale>

typedef signed char byte;

void print(byte value)
{
  // The << operator treats signed char as a mutant char, and tries to
  // print a character. In order to print the value as an integer, you
  // must cast it to an integer type.
  std::cout << "byte=" << static_cast<int>(value) << '\n';
}

void print(short value)
{
  std::cout << "short=" << value << '\n';
}

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
  print(static_cast<short>(0));
  print(static_cast<byte>(0));
  print(static_cast<byte>(255));
  print(static_cast<short>(65535));
  print(32768);
  print(32768L);
  print(-32768);
  print(2147483647);
  print(-2147483647);
  print(2147483648);
  print(9223372036854775807);
  print(-9223372036854775807);
}
