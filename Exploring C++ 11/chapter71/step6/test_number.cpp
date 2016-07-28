#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>

#include <boost/test/auto_unit_test.hpp>

#include "calc_error.hpp"
#include "number.hpp"

BOOST_AUTO_TEST_SUITE(number_test);

BOOST_AUTO_TEST_CASE(test_basics)
{
  number n(1L);
  BOOST_CHECK_EQUAL("1", n.to_string());
  number m(2.0);
  BOOST_CHECK_EQUAL("2", m.to_string());
  m = n;
  BOOST_CHECK_EQUAL("1", m.to_string());
  number x((number(n)));
  BOOST_CHECK_EQUAL("1", x.to_string());
}

BOOST_AUTO_TEST_CASE(test_library)
{
  number i(1), f(2.0), v;
  std::stringstream stream;
  i.save(stream);
  f.save(stream);
  v.save(stream);

  number n1(stream);
  number n2(stream);
  number n3(stream);

  BOOST_CHECK_EQUAL(number(1), n1);
  BOOST_CHECK_EQUAL(number(2), n2);
  BOOST_CHECK_EQUAL("",  n3.to_string());
}

BOOST_AUTO_TEST_CASE(test_compare)
{
  BOOST_CHECK(number(1) < number(2));
  BOOST_CHECK(number(1) <= number(2));
  BOOST_CHECK(number(1) != number(2));
  BOOST_CHECK(number(2) > number(1));
  BOOST_CHECK(number(2) >= number(1));
  BOOST_CHECK(number(2) != number(1));
  BOOST_CHECK(number(1) == number(1));
  BOOST_CHECK(not (number(1) != number(1)));
  BOOST_CHECK(not (number(1) > number(2)));
  BOOST_CHECK(not (number(1) >= number(2)));
  BOOST_CHECK(not (number(1) == number(2)));
  BOOST_CHECK(not (number(2) < number(1)));
  BOOST_CHECK(not (number(2) <= number(1)));
  BOOST_CHECK(not (number(2) == number(1)));

  BOOST_CHECK(number(1) < number(1.5));
  BOOST_CHECK(number(1) <= number(1.5));
  BOOST_CHECK(number(1) != number(1.5));
  BOOST_CHECK(number(1.5) > number(1));
  BOOST_CHECK(number(1.5) >= number(1));
  BOOST_CHECK(number(1.5) != number(1));
  BOOST_CHECK(number(1.0) == number(1));
}

BOOST_AUTO_TEST_CASE(test_add)
{
  number a(1.0), b(2), c;
  c = a + b;
  BOOST_CHECK_EQUAL(number(3), c);
}

BOOST_AUTO_TEST_CASE(test_subtract)
{
  number a(2.0), b(1), c;
  c = a - b;
  BOOST_CHECK_EQUAL(number(1), c);
}

BOOST_AUTO_TEST_CASE(test_multiply)
{
  number a(2.0), b(3), c;
  c = a * b;
  BOOST_CHECK_EQUAL(number(6), c);
}

BOOST_AUTO_TEST_CASE(test_divide)
{
  number a(5), b(2), c;
  c = a / b;
  BOOST_CHECK_EQUAL(number(rational<long>(5, 2)), c);
  a = number(5.0);
  c = a / b;
  BOOST_CHECK_EQUAL(number(2.5), c);
  BOOST_CHECK_THROW(a / number(0), calc_error);
}

BOOST_AUTO_TEST_CASE(test_rational)
{
  number half(rational<long>(1, 2));
  number a(half);
  a = a * number(2);
  BOOST_CHECK_EQUAL(number(1), a);
  a = a / number(3);
  a = a * number(3);
  BOOST_CHECK_EQUAL(number(1), a);
  a = a - half;
  BOOST_CHECK_EQUAL(half, a);
  a = half + a;
  BOOST_CHECK_EQUAL(number(1), a);
}

BOOST_AUTO_TEST_SUITE_END();
