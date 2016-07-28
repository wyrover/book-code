#include <boost/regex.hpp>
#include <string>
#include <iostream>
int main()
{
    std::string str2 = "An array of papers from the academia "
                       "on Alaska area's fauna";
    boost::regex r2("a(\\w*)a");
    boost::smatch matches;
    std::string::const_iterator start = str2.begin(),
                                end = str2.end();

    while (boost::regex_search(start, end, matches, r2)) {
        std::cout << "Matched substring '" << matches.str()
                  << "' following '" << matches.prefix().str()
                  << " preceding '" << matches.suffix().str() << "'\n";
        start = matches[0].second;

        for (size_t s = 1; s < matches.size(); ++s) {
            if (matches[s].matched) {
                std::cout << "Matched substring " << matches[s].str()
                          << " at offset " << matches[s].first - str2.begin()
                          << " of length " << matches[s].length() << '\n';
            }
        }
    }
}

