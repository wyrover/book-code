// Listing 69-1. Qualifying a Member Function with the Base Class Name
#include <iostream>

class base {
public:
   void print(int x) { std::cout << "int: " << x << '\n'; }
};
class derived : public base {
public:
   void print(double x) { std::cout << "double: " << x << '\n'; }
};
int main()
{
   derived d{};
   d.print(3);           // prints double: 3
   d.print(3.0);         // prints double: 3
   d.base::print(3);     // prints int: 3
   d.base::print(3.0);   // prints int: 3
}
