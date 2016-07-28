// Listing 43-9. Testing Various Comparison Algorithms
#include <algorithm>
#include <iostream>
#include <vector>

#include "data.hpp"

int main()
{
  intvector data1{};
  intvector data2{};

  read_data(data1);
  std::cin.clear();
  read_data(data2);

  std::cout << "data1: ";
  write_data(data1);
  std::cout << "data2: ";
  write_data(data2);

  auto data1_end(data1.begin() + std::min(data1.size(), data2.size()));

  std::cout << std::boolalpha;
  std::cout << "equal(data1, data2) = " <<
    equal(data1.begin(), data1_end, data2.begin()) << '\n';

  auto result(mismatch(data1.begin(), data1_end, data2.begin()));

  std::cout << "mismatch(data1, data2) = index " <<
   std::distance(data1.begin(), result.first) << '\n';

  std::cout << "lex_comp(data1, data2) = " <<
    std::lexicographical_compare(data1.begin(), data1.end(),
                                 data2.begin(), data2.end()) << '\n';
}
