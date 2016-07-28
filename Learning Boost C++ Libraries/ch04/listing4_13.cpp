#include <boost/range.hpp>
#include <boost/range/adaptors.hpp>
#include <string>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <cassert>

int main()
{
    std::string str = "funny text";
    auto range = str | boost::adaptors::strided(2);
    boost::to_upper(range);
    assert(str == "FuNnY TeXt");
}

