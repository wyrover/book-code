#include <boost/spirit/include/qi.hpp>
#include <iostream>
namespace qi = boost::spirit::qi;

void print(unsigned int n)
{
    std::cout << n << '\n';
}

int main()
{
    std::string str = "10 20 30 40 50 60";
    auto iter = str.begin();
    bool success = qi::phrase_parse(iter, str.end(),
                                    +qi::uint_[print],
                                    qi::space);
    assert(success);
    assert(iter == str.end());
}

