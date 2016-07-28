#include <boost/spirit/include/qi.hpp>
#include <cassert>
namespace qi = boost::spirit::qi;

int main()
{
    std::string str = "Hello, world!";
    auto iter = str.begin();
    bool success = qi::parse(iter, str.end(), qi::alpha);
    assert(!success);
    assert(iter - str.begin() == 1);
}

