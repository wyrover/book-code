// Listing 69-2. Overloading Named with a using Declaration
#include <iostream>

class base {
public:
   void print(int x) { std::cout << "int: " << x << '\n'; }
};
class derived : public base {
public:
   void print(double x) { std::cout << "double: " << x << '\n'; }
   using base::print;
};
int main()
{
   derived d{};
   d.print(3);            // prints int: 3
   d.print(3.0);          // prints double: 3
}
