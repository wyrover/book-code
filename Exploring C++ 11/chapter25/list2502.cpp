// Listing 25-2. Revealing the Number of Bits in Short and Long Integers 
#include <iostream>
#include <limits>

int main()
{
  std::cout << "bits per int = ";
  if (std::numeric_limits<int>::is_signed)
    std::cout << std::numeric_limits<int>::digits + 1 << '\n';
  else
    std::cout << std::numeric_limits<int>::digits << '\n';

  std::cout << "bits per bool = ";
  if (std::numeric_limits<bool>::is_signed)
    std::cout << std::numeric_limits<bool>::digits + 1 << '\n';
  else
    std::cout << std::numeric_limits<bool>::digits << '\n';

  std::cout << "bits per short int = ";
  if (std::numeric_limits<short>::is_signed)
    std::cout << std::numeric_limits<short>::digits + 1 << '\n';
  else
    std::cout << std::numeric_limits<short>::digits << '\n';

  std::cout << "bits per long int = ";
  if (std::numeric_limits<long>::is_signed)
    std::cout << std::numeric_limits<long>::digits + 1 << '\n';
  else
    std::cout << std::numeric_limits<long>::digits << '\n';

  std::cout << "bits per long long int = ";
  if (std::numeric_limits<long long>::is_signed)
    std::cout << std::numeric_limits<long long>::digits + 1 << '\n';
  else
    std::cout << std::numeric_limits<long long>::digits << '\n';
}
