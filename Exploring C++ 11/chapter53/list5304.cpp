// Listing 53-4. Testing the erase_less Function Template
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <vector>

#include "erase_less.hpp" //Listing 53-3


/// Extract items from a string and store them in a container.
template<class Container>
void read(std::string const& str, Container& cont)
{
  std::istringstream in(str);
  cont.insert(cont.begin(),
              std::istream_iterator<typename Container::value_type>(in),
              std::istream_iterator<typename Container::value_type>());
}

/// Print items from a container to the standard output.
template<class Container>
void print(std::string const& label, Container const& cont)
{
  std::cout << label;
  std::copy(cont.begin(), cont.end(),
            std::ostream_iterator<typename Container::value_type>(std::cout, " "));
  std::cout << '\n';
}

/// Test erase_less by extracting integers from a string into a container
/// and calling erase_less. Print the container before and after.
/// Double-check that the same results obtain with a list and a vector.
void test(std::string const& str)
{
  std::list<int> list{};
  read(str, list);
  print("before: ", list);
  erase_less(list);
  print("after:  ", list);

  std::vector<int> vector{};
  read(str, vector);
  erase_less(vector);

  assert(list.size() == vector.size());
  assert(std::equal(list.begin(), list.end(), vector.begin()));
}

int main()
{
  test("2 3 7 11 13 17 23 29 31 37");
  test("37 31 29 23 17 13 11 7 3 2");
  test("");
  test("42");
  test("10 30 20 40 0 50");
}
