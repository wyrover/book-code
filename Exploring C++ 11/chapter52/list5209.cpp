// Listing 52-9. Examples of using Declarations with Classes
#include <iostream>

class base
{
public:
  void print(int i) { std::cout << "base: " << i << '\n'; }
};

class derived1 : public base
{
public:
  void print(double d) { std::cout << "derived: " << d << '\n'; }
};

class derived2 : public base
{
public:
  using base::print;
  void print(double d) { std::cout << "derived: " << d << '\n'; }
};

int main()
{
  derived1 d1{};
  derived2 d2{};

  d1.print(42);
  d2.print(42);
}
