#include <boost/regex.hpp>
#include <string>
#include <cassert>
int main()
{
    std::string str1 = "Alaska area";
    boost::regex r1("a.*a");
    assert(!boost::regex_match(str1, r1));
}

