#include <boost/config.hpp>
#include <boost/version.hpp>
#include <iostream>

void buildEnvInfo()
{
    std::cout << "Compiler: " << BOOST_COMPILER << '\n'
              << "Platform: " << BOOST_PLATFORM << '\n'
              << "Library: " << BOOST_STDLIB << '\n';
    std::cout << "Boost version: " << BOOST_LIB_VERSION << '['
              << BOOST_VERSION << ']' << '\n';
}

int main()
{
    buildEnvInfo();
}
