#include <vector>
#include <string>
#include <iostream>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/stl/algorithm.hpp>
#include <boost/phoenix/stl/container.hpp>
#include <cassert>

int main()
{
    namespace phx = boost::phoenix;
    using phx::arg_names::arg1;
    std::vector<std::string> greets{ "Hello", "Hola", "Hujambo",
                                     "Hallo" };
    auto finder = phx::find(greets, arg1);
    auto it = finder("Hujambo");
    assert(phx::end(greets)() != it);
    std::cout << *it << '\n';
    assert(++it != greets.end());
    std::cout << *it << '\n';
}

