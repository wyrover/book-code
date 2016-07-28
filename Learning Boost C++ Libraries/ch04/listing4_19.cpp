#include <iostream>
#include <boost/tokenizer.hpp>
#include <string>

int main()
{
    std::string input =
        "God knows, I've never been a spiritual man!";
    boost::tokenizer<> tokenizer(input);

    for (boost::tokenizer<>::iterator token = tokenizer.begin();
         token != tokenizer.end(); ++token) {
        std::cout << *token << '\n';
    }
}

