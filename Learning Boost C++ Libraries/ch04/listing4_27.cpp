#include <boost/regex.hpp>
#include <string>
#include <iostream>

int main()
{
    std::string str2 = "An array of papers from the academia "
                       "on Alaska area's fauna";
    boost::regex r2("a\\w*a");
    boost::smatch matches;
    std::string::const_iterator start = str2.begin(),
                                end = str2.end();

    while (boost::regex_search(start, end, matches, r2)) {
        std::cout << "Matched substring " << matches.str()
                  << " at offset " << matches[0].first - str2.begin()
                  << " of length " << matches[0].length() << '\n';
        start = matches[0].second;
    }
}

