#include <boost/regex.hpp>
#include <cassert>

int main()
{
    std::string str4 = "England's Queen, India's President, "
                       "people's choice";
    boost::regex r4("(\\w+)'s\\s+(\\w+)");
    std::string rep = boost::regex_replace(str4, r4, "\\2 of \\1");
    assert(rep == "Queen of England, President of India, "
           "choice of people");
}

