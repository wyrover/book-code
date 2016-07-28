// Listing 68-4. Argument-Dependent Name Lookup
#include <cmath>
#include <iostream>
#include "rational.hpp"

namespace data {
  template<class T>
  struct point {
    T x, y;
  };
  template<class Ch, class Tr, class T>
  std::basic_ostream<Ch, Tr>& operator<<(std::basic_ostream<Ch, Tr>& stream, point<T> const& pt)
  {
    stream << '(' << pt.x << ", " << pt.y << ')';
    return stream;
  }
  template<class T>
  T abs(point<T> const& pt) {
    using namespace std;
    return sqrt(pt.x * pt.x + pt.y * pt.y);
  }
}

namespace numeric {
   template<class T>
   rational<T> sqrt(rational<T> r)
   {
     using std::sqrt;
     return rational<T>{sqrt(static_cast<double>(r))};
   }
}

int main()
{
   using namespace std;
   data::point<numeric::rational<int>> a{ numeric::rational<int>{1, 2}, numeric::rational<int>{2, 4} };
   std::cout << "abs(" << a << ") = " << abs(a) << '\n';
}
