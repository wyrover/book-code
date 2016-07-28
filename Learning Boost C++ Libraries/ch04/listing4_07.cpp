#include <boost/algorithm/string.hpp>
#include <string>
#include <cassert>

int main()
{
    std::string str1 = "Find the Cost of Freedom";
    std::string str2 = boost::to_lower_copy(str1);
    assert(str1 != str2);
    boost::to_lower(str1);
    assert(str1 == str2);
    assert(str1 == "find the cost of freedom");
}

