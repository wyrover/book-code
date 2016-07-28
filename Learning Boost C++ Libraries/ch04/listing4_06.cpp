#include <boost/algorithm/string.hpp>
#include <string>
#include <cassert>

int main()
{
    std::string input = "linearize";
    std::string test = "near";
    assert(boost::contains(input, test));
}

