#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>

#include <boost/test/unit_test.hpp>

#include "parse.hpp"

BOOST_AUTO_TEST_SUITE(parser_test);

BOOST_AUTO_TEST_CASE(test_number)
{
  std::istringstream in("1\n3.14159\n1.23e45\n45.67e+8\n");
  std::ostringstream out;
  parse_loop(in, out);
  BOOST_CHECK_EQUAL("> 1\n> 3.14159\n> 1.23e+45\n> 4.567e+09\n> ", out.str());
}

BOOST_AUTO_TEST_CASE(test_negative)
{
  std::istringstream in("-1\n-3.14159\n-1.23e-45\n-34.56e-7\n");
  std::ostringstream out;
  parse_loop(in, out);
  BOOST_CHECK_EQUAL("> -1\n> -3.14159\n> -1.23e-45\n> -3.456e-06\n> ", out.str());
}

BOOST_AUTO_TEST_CASE(test_add)
{
  std::istringstream in("1 + 2\n1 + 2 + 3");
  std::ostringstream out;
  parse_loop(in, out);
  BOOST_CHECK_EQUAL("> 3\n> 6\n> ", out.str());
}

BOOST_AUTO_TEST_CASE(test_subtract)
{
  std::istringstream in("1 - 2\n5 - 1 - 2");
  std::ostringstream out;
  parse_loop(in, out);
  BOOST_CHECK_EQUAL("> -1\n> 2\n> ", out.str());
}

BOOST_AUTO_TEST_CASE(test_multiply)
{
  std::istringstream in("1 * 2\n5 * 2 * 1.5");
  std::ostringstream out;
  parse_loop(in, out);
  BOOST_CHECK_EQUAL("> 2\n> 15\n> ", out.str());
}

BOOST_AUTO_TEST_CASE(test_divide)
{
  std::istringstream in("1 / 2\n10 / 2 / 2");
  std::ostringstream out;
  parse_loop(in, out);
  BOOST_CHECK_EQUAL("> 0.5\n> 2.5\n> ", out.str());
}

BOOST_AUTO_TEST_CASE(test_mix)
{
  std::istringstream in(" 1.5 * 2 + 3 / 1.5 - (10 - 3) + 2 * -1");
  std::ostringstream out;
  parse_loop(in, out);
  BOOST_CHECK_EQUAL("> -4\n> ", out.str());
}

BOOST_AUTO_TEST_CASE(test_var)
{
  std::istringstream in(" var half = 1 / 2\nvar one=1.0\n one + half");
  std::ostringstream out;
  parse_loop(in, out);
  BOOST_CHECK_EQUAL("> 0.5\n> 1\n> 1.5\n> ", out.str());
}

BOOST_AUTO_TEST_CASE(test_predefined_vars)
{
  std::istringstream in(" pi - e");
  std::ostringstream out;
  parse_loop(in, out);
  BOOST_CHECK_EQUAL("> 0.423311\n> ", out.str());
}

BOOST_AUTO_TEST_CASE(test_error1)
{
  std::istringstream in("1+++2");
  std::ostringstream out;
  parse_loop(in, out);
  BOOST_CHECK_EQUAL("> syntax error: expected a primary, but got +\n> ", out.str());
}

BOOST_AUTO_TEST_CASE(test_error2)
{
  std::istringstream in("1..2");
  std::ostringstream out;
  parse_loop(in, out);
  BOOST_CHECK_EQUAL("> syntax error: expected digit after decimal point, got '.'\n> ", out.str());
}

BOOST_AUTO_TEST_CASE(test_error3)
{
  std::istringstream in("1 @ 2");
  std::ostringstream out;
  parse_loop(in, out);
  BOOST_CHECK_EQUAL("> syntax error: expected digit, got '@'\n> ", out.str());
}

BOOST_AUTO_TEST_CASE(test_error4)
{
  std::istringstream in("(1 + 2");
  std::ostringstream out;
  parse_loop(in, out);
  BOOST_CHECK_EQUAL("> syntax error: EOF when expecting ')'\n> ", out.str());
}

BOOST_AUTO_TEST_SUITE_END();
