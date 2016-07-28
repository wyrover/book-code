#include <iostream>
#include <boost/tokenizer.hpp>
#include <string>

int main()
{
    std::string input =
        "/Alon Ben-Ari/-35-11~/5 Zamenhoff St., Tel Aviv";
    typedef boost::tokenizer<boost::escaped_list_separator<char> >
    tokenizer;
    boost::escaped_list_separator<char> sep('~', '-', '/');
    tokenizer mytokenizer(input, sep);

    for (auto& tok : mytokenizer) {
        std::cout << tok << '\n';
    }
}

