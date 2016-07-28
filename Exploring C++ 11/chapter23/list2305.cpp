// Listing 23-5. Using the mutable Keyword in a Lambda
#include <iostream>

int main()
{
   int x{0};
   auto lambda = [x](int y) mutable {
      x = 1;
      y = 2;
      return x + y;
   };
   int local{0};
   std::cout << lambda(local) << ", " << x << ", " << local << '\n';
}
