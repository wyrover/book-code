#include <boost/tokenizer.hpp>
#include <string>
#include <iostream>

int main()
{
    std::string input =
        "God knows, I've never been a spiritual man!";
    boost::char_separator<char> sep(" \t,.!?;./\"(){}[]<>");
    typedef boost::tokenizer<boost::char_separator<char> >
    tokenizer;
    tokenizer mytokenizer(input, sep);

    for (auto& token : mytokenizer) {
        std::cout << token << '\n';
    }
}

