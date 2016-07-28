// Listing 18-3. Using the Native Locale for Output
#include <iostream>
#include <locale>

int main()
{
  std::cout.imbue(std::locale{""});

  int sum{0};
  int x{};
  while (std::cin >> x)
    sum = sum + x;
  std::cout << "sum = " << sum << '\n';
}
