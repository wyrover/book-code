#include <boost/variant.hpp>
#include <string>
#include <cassert>
#include <iostream>

int main()
{
    boost::variant<std::string, int> v1;
    v1 = "19937";                    // sets string
    int i1;

    try {
        i1 = boost::get<int>(v1);      // will fail, throw
    } catch (std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    int *pi = boost::get<int>(&v1);  // will return null
    assert(pi == 0);
    size_t index = v1.which();        // returns 0
}

