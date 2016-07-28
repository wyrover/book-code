#include <iostream>
#include <boost/tokenizer.hpp>
#include <string>

int main()
{
    std::string input = "Amit Gupta,70,\"\\\"Nandanvan\\\", "
                        "Ghole Road, Pune, India\"";
    typedef boost::tokenizer<boost::escaped_list_separator<char> >
    tokenizer;
    tokenizer mytokenizer(input);

    for (auto& tok : mytokenizer) {
        std::cout << tok << '\n';
    }
}

