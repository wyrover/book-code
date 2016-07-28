#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>

#include <boost/test/unit_test.hpp>

#include "node.hpp"
#include "variables.hpp"

BOOST_AUTO_TEST_SUITE(node_test);

BOOST_AUTO_TEST_CASE(test_basics)
{
  node n(1.0);
  BOOST_CHECK_EQUAL(1.0, n.evaluate());
  BOOST_CHECK_EQUAL("1", n.to_string());
  node s("ident");
  BOOST_CHECK_EQUAL("ident", s.to_string());
}

BOOST_AUTO_TEST_CASE(test_negate)
{
  node n(1.0);
  node neg('-', n);
  BOOST_CHECK_EQUAL(-1.0, neg.evaluate());
}

BOOST_AUTO_TEST_CASE(test_add)
{
  node a(1.0), b(2.0), c(3.0);
  BOOST_CHECK_EQUAL(3.0, node(a, '+', b).evaluate());
  BOOST_CHECK_EQUAL(6.0, node(node(a, '+', b), '+', c).evaluate());
}

BOOST_AUTO_TEST_CASE(test_subtract)
{
  node a(1.0), b(2.0), c(3.0);
  BOOST_CHECK_EQUAL(1.0, node(c, '-', b).evaluate());
  BOOST_CHECK_EQUAL(-4.0, node(node(a, '-', b), '-', c).evaluate());
}

BOOST_AUTO_TEST_CASE(test_multiply)
{
  node a(1.0), b(2.0), c(3.0);
  BOOST_CHECK_EQUAL(3.0, node(a, '*', c).evaluate());
  BOOST_CHECK_EQUAL(6.0, node(node(a, '*', b), '*', c).evaluate());
}

BOOST_AUTO_TEST_CASE(test_divide)
{
  node a(1.0), b(2.0), c(6.0);
  BOOST_CHECK_EQUAL(0.5, node(a, '/', b).evaluate());
  BOOST_CHECK_EQUAL(3.0, node(node(c, '/', a), '/', b).evaluate());
}

BOOST_AUTO_TEST_CASE(test_mix)
{
  node a(node(1.5), '*', node(2.0));
  BOOST_CHECK_EQUAL(3.0, a.evaluate());

  node b(node(3.0), '/', node(1.5));
  BOOST_CHECK_EQUAL(2.0, b.evaluate());

  node c(a, '+', b);
  BOOST_CHECK_EQUAL(5.0, c.evaluate());

  node d(node(10), '-', node(3));
  BOOST_CHECK_EQUAL(7.0, d.evaluate());

  node e('-', node(1.0));
  BOOST_CHECK_EQUAL(-1.0, e.evaluate());

  node f(node(2), '*', e);
  BOOST_CHECK_EQUAL(-2.0, f.evaluate());

  node g(d, '+', f);
  BOOST_CHECK_EQUAL(5.0, g.evaluate());

  node h(c, '-', g);
  BOOST_CHECK_EQUAL(0.0, h.evaluate());
}

BOOST_AUTO_TEST_CASE(test_var)
{
  node n(node(node("half"), node(node(1), '/', node(2))));
  BOOST_CHECK_EQUAL(0.5, n.evaluate());
  BOOST_CHECK_EQUAL(0.5, get_variable("half"));
}

BOOST_AUTO_TEST_CASE(test_copy)
{
  node n(node(node(1), '+', node(2)));
  {
    node m(n);
    n = node('-', m);
    BOOST_CHECK_EQUAL(3.0, m.evaluate());
  }
  BOOST_CHECK_EQUAL(-3.0, n.evaluate());
}

BOOST_AUTO_TEST_SUITE_END();
