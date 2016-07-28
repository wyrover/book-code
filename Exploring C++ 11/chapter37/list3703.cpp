// Listing 37-3. Order of Calling Destructors
#include <iostream>

class base
{
public:
  base()  { std::cout << "base\n"; }
  ~base() { std::cout << "~base\n"; }
};

class middle : public base
{
public:
  middle()  { std::cout << "middle\n"; }
  ~middle() { std::cout << "~middle\n"; }
};

class derived : public middle
{
public:
  derived()  { std::cout << "derived\n"; }
  ~derived() { std::cout << "~derived\n"; }
};

int main()
{
  derived d;
}
