#include <boost/spirit/include/qi.hpp>
#include <cassert>
namespace qi = boost::spirit::qi;

int main()
{
    std::string str = "Hello, world!";
    auto iter = str.begin();
    bool success = qi::parse(iter, str.end(),
                             qi::string("Hello") >> qi::string("world"));
    assert(!success);
    iter = str.begin();
    success = qi::phrase_parse(iter, str.end(),
                               qi::string("Hello") >> qi::string("world"),
                               +(qi::space | qi::punct));
    assert(success);
    assert(iter - str.begin() == str.size());
}

