// Listing 41-7. Rewriting main.cpp to #include the generate_id.hpp File
#include <iostream>

#include "generate_id.hpp"

int main()
{
  generate_id gen{};           // Create an ID generator
  for (int i{0}; i != 10; ++i)
    std::cout << gen.next() << '\n';
}
