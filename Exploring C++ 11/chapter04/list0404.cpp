// Listing 4-4. Printing Information That Is Stored in Variables
#include <iostream>
#include <string>

int main()
{
   std::string shape{"Triangle"};
   int sides{3};

   std::cout << "Shape\t\tSides\n" <<
                "-----\t\t-----\n";
   std::cout << "Square\t\t" << 4 << '\n' <<
                "Circle\t\t?\n";
   std::cout << shape << '\t' << sides << '\n';
}
