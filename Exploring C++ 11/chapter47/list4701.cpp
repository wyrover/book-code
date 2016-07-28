// Listing 47-1. Testing the fixed Class
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "test.hpp"
#include "fixed.hpp"

int main()
{
  fixed f1{};
  test_equal(f1.value(), 0);
  fixed f2{1};
  test_equal(f2.value(), 10000);
  fixed f3{3, 14162};
  test_equal(f3.value(), 31416);
  fixed f4{2, 14159265};
  test_equal(f4.value(), 21416);
  test_equal(f2 + f4, f1 + f3);
  test(f2 + f4 <= f1 + f3);
  test(f2 + f4 >= f1 + f3);
  test(f1 < f2);
  test(f1 <= f2);
  test(f1 != f2);
  test(f2 > f1);
  test(f2 >= f1);
  test(f2 != f1);

  test_equal(f2 + f4, f3 - f1);
  test_equal(f2 * f3, f3);
  test_equal(f3 / f2, f3);
  f4 += f2;
  test_equal(f3, f4);
  f4 -= f1;
  test_equal(f3, f4);
  f4 *= f2;
  test_equal(f3, f4);
  f4 /= f2;
  test_equal(f3, f4);

  test_equal(-f4, f1 - f4);
  test_equal(-(-f4), f4);
  --f4;
  test_equal(f4 + 1, f3);
  f4--;
  test_equal(f4 + 2, f3);
  ++f4;
  test_equal(f4 + 1, f3);
  f4++;
  test_equal(f4, f3);
  ++f3;
  test_equal(++f4, f3);
  test_equal(f4--, f3);
  test_equal(f4++, --f3);
  test_equal(--f4, f3);

  test_equal(f4 / f3, f2);
  test_equal(f4 - f3, f1);

  test_equal(f4.to_string(), "3.1416");
  test_equal(f4.integer(), 3);
  f4 += fixed{0,4584};
  test_equal(f4, 3.6);
  test_equal(f4.integer(), 3);
  test_equal(f4.round(), 4);

  test_equal(f3.integer(), 3);
  test_equal((-f3).integer(), -3);
  test_equal(f3.fraction(), 1416);
  test_equal((-f3).fraction(), 1416);

  test_equal(fixed{7,4999}.round(), 7);
  test_equal(fixed{7,5000}.round(), 8);
  test_equal(fixed{7,5001}.round(), 8);
  test_equal(fixed{7,4999}.round(), 7);
  test_equal(fixed{8,5000}.round(), 8);
  test_equal(fixed{8,5001}.round(), 9);

  test_equal(fixed{123,2345500}, fixed(123,2346));
  test_equal(fixed{123,2345501}, fixed(123,2346));
  test_equal(fixed{123,2345499}, fixed(123,2345));
  test_equal(fixed{123,2346500}, fixed(123,2346));
  test_equal(fixed{123,2346501}, fixed(123,2347));
  test_equal(fixed{123,2346499}, fixed(123,2346));
  test_equal(fixed{123,2346400}, fixed(123,2346));
  test_equal(fixed{123,2346600}, fixed(123,2347));

  test_equal(fixed{-7,4999}.round(), -7);
  test_equal(fixed{-7,5000}.round(), -8);
  test_equal(fixed{-7,5001}.round(), -8);
  test_equal(fixed{-7,4999}.round(), -7);
  test_equal(fixed{-8,5000}.round(), -8);
  test_equal(fixed{-8,5001}.round(), -9);

  test_equal(fixed{-3.14159265}.value(), -31416);
  test_equal(fixed{123,456789}.value(), 1234568);
  test_equal(fixed{123,4}.value(), 1230004);
  test_equal(fixed{-10,1111}.value(), -101111);

  std::ostringstream out{};
  out << f3 << " 3.14159265 " << fixed(-10,12) << " 3 421.4 end";
  fixed f5{};
  std::istringstream in{out.str()};
  test(in >> f5);
  test_equal(f5, f3);
  test(in >> f5);
  test_equal(f5, f3);
  test(in >> f5);
  test_equal(f5.value(), -100012);
  test(in >> f5);
  test_equal(f5.value(), 30000);
  test(in >> f5);
  test_equal(f5.value(), 4214000);
  test(not (in >> f5));

  test_equal(fixed{31.4159265}, fixed{31, 4159});
  test_equal(fixed{31.41595}, fixed{31, 4160});

  bool okay{false};
  try {
    fixed f6{1, -1};
  } catch (std::invalid_argument const& ex) {
    okay = true;
  } catch (...) {
  }
  test(okay);
}
