#include <boost/regex.hpp>
#include <string>
#include <iostream>

int main()
{
    std::string str3 = "animal=Llama lives_in=Chile "
                       "and is related_to=vicuna";
    boost::regex r3("(\\w+)=(\\w+)");
    int subindx[] = {2, 1};
    boost::sregex_token_iterator tokit(str3.begin(), str3.end(),
                                       r3, subindx), tokend;

    while (tokit != tokend) {
        std::cout << *tokit++ << '\n';
    }

    std::cout << '\n';
}

