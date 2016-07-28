// Listing 64-8. Exploring How Negative and Unsigned Values Are Shifted
#include <iostream>
#include <string>

template<class T>
void print(std::string const& label, T value)
{
   std::cout << label << " = ";
   std::cout << std::dec << value << " = ";
   std::cout.width(8);
   std::cout.fill('0');
   std::cout << std::hex << std::internal << std::showbase << value << '\n';
}

int main()
{
   int i{~0}; // all bits set to 1; on most systems, ~0 == -1
   unsigned int u{~0u}; // all bits set to 1
   print("int >> 15", i >> 15);
   print("unsigned >> 15", u >> 15);
}
