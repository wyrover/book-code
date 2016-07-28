// Listing 25-5. User-Defined Literal
#include <iostream>

short operator "" _S(unsigned long long value)
{
    return static_cast<short>(value);
}

void print(short s)
{
   std::cout << "short=" << s << '\n';
}

void print(int i)
{
   std::cout << "int=" << i << '\n';
}

int main()
{
   print(42);
   print(42_S);
}
