#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>
#include <iostream>

int main()
{
    namespace phx = boost::phoenix;
    double eX;
    auto x = phx::ref(eX);
    eX = 10.0;
    std::cout << (x + 1 / x)() << '\n';            // prints 10.1
    std::cout << ((x * x - x + 4) / (2 * x))() << '\n'; // prints 4.7
}

