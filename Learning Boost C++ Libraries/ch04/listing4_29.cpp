#include <boost/regex.hpp>
#include <string>
#include <iostream>

int main()
{
    std::string str2 = "An array of papers from the academia "
                       "on Alaska area's fauna";
    boost::regex r1("\\ba\\w*a\\b", boost::regex::icase);
    boost::sregex_iterator rit(str2.begin(), str2.end(), r1), rend;

    while (rit != rend) {
        std::cout << *rit++ << '\n';
    }
}

