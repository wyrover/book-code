/** @file test.cpp */
/** Listing 45-1. Testing the fixed Class */
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <stdexcept>

#include "test.hpp"
#include "fixed.hpp"

int main()
{
  fixed f1;
  TEST(f1.value() == 0);
  fixed f2(1);
  TEST(f2.value() == 10000);
  fixed f3(3, 14162);
  TEST(f3.value() == 31416);
  fixed f4(2, 14159265);
  TEST(f4.value() == 21416);
  TEST(f2 + f4 == f1 + f3);
  TEST(f2 + f4 <= f1 + f3);
  TEST(f2 + f4 >= f1 + f3);
  TEST(f1 < f2);
  TEST(f1 <= f2);
  TEST(f1 != f2);
  TEST(f2 > f1);
  TEST(f2 >= f1);
  TEST(f2 != f1);

  TEST(f2 + f4 == f3 - f1);
  TEST(f2 * f3 == f3);
  TEST(f3 / f2 == f3);
  f4 += f2;
  TEST(f3 == f4);
  f4 -= f1;
  TEST(f3 == f4);
  f4 *= f2;
  TEST(f3 == f4);
  f4 /= f2;
  TEST(f3 == f4);

  TEST(-f4 == f1 - f4);
  TEST(-(-f4) == f4);
  --f4;
  TEST(f4 + 1 == f3);
  f4--;
  TEST(f4 + 2 == f3);
  ++f4;
  TEST(f4 + 1 == f3);
  f4++;
  TEST(f4 == f3);
  ++f3;
  TEST(++f4 == f3);
  TEST(f4-- == f3);
  TEST(f4++ == --f3);
  TEST(--f4 == f3);

  TEST(f4 / f3 == f2);
  TEST(f4 - f3 == f1);

  TEST(f4.as_string() == "3.1416");
  TEST(f4.integer() == 3);
  f4 += fixed(0,4584);
  TEST(f4 == 3.6);
  TEST(f4.integer() == 3);
  TEST(f4.round() == 4);

  TEST(f3.integer() == 3);
  TEST((-f3).integer() == -3);
  TEST(f3.fraction() == 1416);
  TEST((-f3).fraction() == 1416);

  TEST(fixed(7,4999).round() == 7);
  TEST(fixed(7,5000).round() == 8);
  TEST(fixed(7,5001).round() == 8);
  TEST(fixed(7,4999).round() == 7);
  TEST(fixed(8,5000).round() == 8);
  TEST(fixed(8,5001).round() == 9);

  TEST(fixed(123,2345500) == fixed(123,2346));
  TEST(fixed(123,2345501) == fixed(123,2346));
  TEST(fixed(123,2345499) == fixed(123,2345));
  TEST(fixed(123,2346500) == fixed(123,2346));
  TEST(fixed(123,2346501) == fixed(123,2347));
  TEST(fixed(123,2346499) == fixed(123,2346));
  TEST(fixed(123,2346400) == fixed(123,2346));
  TEST(fixed(123,2346600) == fixed(123,2347));

  TEST(fixed(-7,4999).round() == -7);
  TEST(fixed(-7,5000).round() == -8);
  TEST(fixed(-7,5001).round() == -8);
  TEST(fixed(-7,4999).round() == -7);
  TEST(fixed(-8,5000).round() == -8);
  TEST(fixed(-8,5001).round() == -9);

  TEST(fixed(-3.14159265).value() == -31416);
  TEST(fixed(123,456789).value() == 1234568);
  TEST(fixed(123,4).value() == 1230004);
  TEST(fixed(-10,1111).value() == -101111);

  std::ostringstream out;
  out << f3 << " 3.14159265 " << fixed(-10,12) << " 3 421.4 end";
  fixed f5;
  std::istringstream in(out.str());
  TEST(in >> f5);
  TEST(f5 == f3);
  TEST(in >> f5);
  TEST(f5 == f3);
  TEST(in >> f5);
  TEST(f5.value() == -100012);
  TEST(in >> f5);
  TEST(f5.value() == 30000);
  TEST(in >> f5);
  TEST(f5.value() == 4214000);
  TEST(not (in >> f5));

  TEST(fixed(31.4159265) == fixed(31, 4159));
  TEST(fixed(31.41595) == fixed(31, 4160));

  bool okay(false);
  try {
    fixed f6(1, -1);
  } catch (std::invalid_argument const& ex) {
    okay = true;
  } catch (...) {
  }
  TEST(okay);
}
