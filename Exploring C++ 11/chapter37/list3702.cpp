// Listing 37-2. Printing Messages from Constructors to Illustrate Order of Construction
#include <iostream>

class base
{
public:
  base() { std::cout << "base\n"; }
};

class middle : public base
{
public:
  middle() { std::cout << "middle\n"; }
};

class derived : public middle
{
public:
  derived() { std::cout << "derived\n"; }
};

int main()
{
  derived d;
}
