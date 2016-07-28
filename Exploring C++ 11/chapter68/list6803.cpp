// Listing 68-3. Unqualified Name Lookup
#include <iostream>

namespace outer {
   namespace inner {
      struct point { int x, y; };
      inline std::ostream& operator<<(std::ostream& stream, point const& p)
      {
         stream << '(' << p.x << ", " << p.y << ')';
         return stream;
      }
   }
}

typedef int Integer;

int main()
{
   const int multiplier{2};
   for (Integer i : { 1, 2, 3}) {
      outer::inner::point p{ i, i * multiplier };
      std::cout << p << '\n';
   }
}
