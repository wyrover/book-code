#include <iostream>

// Listing 64-9. Mixing Signed and Unsigned Integers
void show(unsigned u)
{
   std::cout << u << '\n';
}

int main()
{
   int i{-1};
   std::cout << i << '\n';
   show(i);
}
