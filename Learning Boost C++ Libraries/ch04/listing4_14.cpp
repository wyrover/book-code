#include <boost/algorithm/string.hpp>
#include <string>
#include <iostream>
#include <cassert>

bool isNewline(char c)
{
    return c == '\n';
}

int main()
{
    std::string input = "  Hello  ";
    std::string input2 = "Hello   \n";
    boost::trim(input);
    boost::trim_right_if(input2, isNewline);
    assert(*(input.end() - 1) != ' ');
    assert(*(input2.end() - 1) != '\n' &&
           *(input2.end() - 1) == ' ');
}

