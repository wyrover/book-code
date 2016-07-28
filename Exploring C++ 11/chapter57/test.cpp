/** @file test.cpp */
/** Listing 45-1. Testing the fixed Class */
#include <iostream>
#include <istream>
#include <locale>
#include <ostream>
#include <sstream>
#include <stdexcept>

#include "test.hpp"
#include "currency.hpp"

int main(int argc, char** argv)
{
  if (argc >= 2)
    std::locale::global(std::locale(argv[1]));
  else
    std::locale::global(std::locale(""));
  std::cin.imbue(std::locale());
  std::cout.imbue(std::locale());
  typedef currency<long,4> currency4;
  typedef currency<long,2> currency2;
  currency2 c1;
  TEST(c1.value().value() == 0);
  currency2 c2(1L);
  TEST(c2.value().value() == 100);
  currency2 c3(3, 1416);
  TEST(c3.value().value() == 314);
  currency2 c4(6, 275);
  TEST(c4.value().value() == 628);
  currency2 c5(5, 279);
  TEST(c5.value().value() == 528);
  TEST(c3 + c3 == c1 + c4);
  TEST(c3 + c3 <= c1 + c4);
  TEST(c3 + c3 >= c1 + c4);
  TEST(c1 < c2);
  TEST(c1 <= c2);
  TEST(c1 != c2);
  TEST(c2 > c1);
  TEST(c2 >= c1);
  TEST(c2 != c1);
  TEST(c2 / 2L == currency2(0, 50));

  TEST(c4 - c5 == c2);
  TEST(c3 * 2L == c4);
  TEST(c4 / 2L == c3);
  c5 += c2;
  TEST(c5 == c4);
  c5 /= 2L;
  TEST(c3 == c5);
  c3 *= 2L;
  TEST(c3 == c4);
  c3 -= c5;
  TEST(c3 == c5);

  TEST(-c4 == c1 - c4);
  TEST(-(-c4) == c4);
  TEST(c3 + c5 == --c4 + c2);
  TEST(c3 + c5 == c4 + c2);
  TEST(c3 + c5 == c4-- + c2);
  TEST(c3 + c5 == c4 + c2 + c2);
  TEST(c3 + c5 == ++c4 + c2);
  TEST(c3 + c5 == c4 + c2);
  TEST(c3 + c5 == c4++ + c2);
  TEST(c3 + c5 == c4);

  c2 *= 2L;
  TEST(c4 / c2 == rational<long>(314, 100));
  TEST((c4 /= 2L) == c5);
  TEST(c4 == c5);

  TEST(c4.as_string() == "3.14");
  TEST(c4.integer() == 3);
  c4 += currency2(-1,8);
  TEST((c4 == currency2(2.06)));
  TEST(c4.integer() == 2);
  TEST(c4.round() == 2);
  c4 += c2 / 2L;
  TEST(c4.round() == 3);

  TEST(c3.integer() == 3);
  TEST((-c3).integer() == -3);
  TEST(c3.fraction() == 14);
  TEST((-c3).fraction() == 14);

  TEST(currency4(7,4999).round() == 7L);
  TEST(currency4(7,5000).round() == 8L);
  TEST(currency4(7,5001).round() == 8L);
  TEST(currency4(7,4999).round() == 7L);
  TEST(currency4(8,5000).round() == 8L);
  TEST(currency4(8,5001).round() == 9L);

  TEST(currency4(123,2345500) == currency4(123,2346));
  TEST(currency4(123,2345501) == currency4(123,2346));
  TEST(currency4(123,2345499) == currency4(123,2345));
  TEST(currency4(123,2346500) == currency4(123,2346));
  TEST(currency4(123,2346501) == currency4(123,2347));
  TEST(currency4(123,2346499) == currency4(123,2346));
  TEST(currency4(123,2346400) == currency4(123,2346));
  TEST(currency4(123,2346600) == currency4(123,2347));

  TEST(currency4(-3.14159265).value().value() == -31416L);
  TEST(currency4(123,456789).value().value() == 1234568L);
  TEST(currency4(123,4).value().value() == 1230004L);
  TEST(currency4(-10,1111).value().value() == -101111L);

  std::ostringstream out;
  c3 = currency2(3, 14);
  out << std::showbase << c3 << " 3.14 " << currency2(-10,12) << " 3.00 421.40 end";
  currency2 c6;
  std::istringstream in(out.str());
  TEST(in >> c6);
  TEST(c6 == c3);
  c6 = currency2();
  TEST(in >> c6);
  TEST(c6 == c3);
  TEST(in >> c6);
  TEST(c6.value() == -1012L);
  TEST(in >> c6);
  TEST(c6.value() == 300L);
  TEST(in >> c6);
  TEST(c6.value() == 42140L);
  TEST(not (in >> c6));

  TEST(currency2(31.59265) == currency2(31, 59));
  TEST(currency2(31.595) == currency2(31, 60));

  // Adjust the following test to match your native locale.
  currency2 big(1234567, 89);
  TEST(big.as_string() == "1,234,567.89");

  bool okay(false);
  try {
    currency2 c7(1, -1);
  } catch (std::invalid_argument const& ex) {
    okay = true;
  } catch (std::exception const& ex) {
    std::cerr << "wrong exception: " << ex.what() << '\n';
  }
  TEST(okay);
}
