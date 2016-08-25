#include <iostream>
#include <string>
#include <boost/regex.hpp>

int main()
{
    std::string s = "who,lives:in-a,pineapple    under the sea?";
    boost::regex re(",|:|-|\\s+");       // Create the reg exp
    boost::sregex_token_iterator         // Create an iterator using a
    p(s.begin(), s.end(), re, -1);     // sequence and that reg exp
    boost::sregex_token_iterator end;    // Create an end-of-reg-exp

    // marker
    while (p != end)
        std::cout << *p++ << '\n';
}
