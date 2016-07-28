// Listing 44-2. Advancing an Iterator
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

#include "data.hpp"   // see Listing 43-2.

#include "sequence.hpp"   // see Listing 42-6.


int main()
{
  intvector data(10);
  std::generate(data.begin(), data.end(), sequence{0, 2}); // fill with even numbers 
  intvec_iterator iter{data.begin()};
  std::advance(iter, 4);
  std::cout << *iter << ", ";
  iter = std::prev(iter, 2);
  std::cout << *iter << '\n';
}
