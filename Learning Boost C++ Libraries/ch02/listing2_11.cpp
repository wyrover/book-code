#include <boost/lexical_cast.hpp>
#include <cassert>
#include <exception>
#include <iostream>

int main()
{
    std::string str = "1234";

    try {
        int n = boost::lexical_cast<int>(str);
        assert(n == 1234);
    } catch (std::exception& e) {
        std::cout << e.what() << '\n';
    }
}
