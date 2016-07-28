#include <boost/algorithm/string.hpp>
#include <string>
#include <iostream>

int main()
{
    std::string str = "The application tried to read from an "
                      "invalid address at 0xbeeffed";
    auto token = boost::find_token(str, boost::is_xdigit(),
                                   boost::token_compress_on);

    while (token.begin() != token.end()) {
        if (boost::size(token) > 3) {
            std::cout << token << '\n';
        }

        auto remnant = boost::make_iterator_range(token.end(),
                       str.end());
        token = boost::find_token(remnant, boost::is_xdigit(),
                                  boost::token_compress_on);
    }
}

