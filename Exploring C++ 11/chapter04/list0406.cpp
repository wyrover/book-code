// Listing 4-6. Demonstrating Uninitialized Variables
#include <iostream>
#include <string>

int main()
{
   std::string shape;
   int sides;

   std::cout << "Shape\t\tSides\n" <<
                "-----\t\t-----\n";
   std::cout << "Square\t\t" << 4 << '\n' <<
                "Circle\t\t?\n";
   std::cout << shape << '\t' << sides << '\n';
}
