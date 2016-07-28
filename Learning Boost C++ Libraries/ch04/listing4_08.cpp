#include <boost/algorithm/string.hpp>
#include <string>
#include <iostream>

int main()
{
    const char *haystack = "Mary had a little lamb";
    const char *needles[] = {"little", "Little", 0};

    for (int i = 0; needles[i] != 0; ++i) {
        auto ret = boost::find_first(haystack, needles[i]);

        if (ret.begin() == ret.end()) {
            std::cout << "String [" << needles[i] << "] not found in"
                      << " string [" << haystack << "\n";
        } else {
            std::cout << "String [" << needles[i] << "] found at "
                      << "offset " << ret.begin() - haystack
                      << " in string [" << haystack << "\n";
        }

        std::cout << "'" << ret << "'" << '\n';
    }
}

