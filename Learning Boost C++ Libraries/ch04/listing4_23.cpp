#include <boost/tokenizer.hpp>
#include <string>
#include <iostream>

int main()
{
    std::string input =
        "201408091403290000001881303614419ABNANL2AWSSDEUTDEMM72"
        "0000000412000EUR";
    int lengths[] = {8, 9, 16, 11, 11, 12, 13};
    boost::offset_separator ofs(lengths, lengths + 7);
    typedef boost::tokenizer<boost::offset_separator> tokenizer;
    tokenizer mytokenizer(input, ofs);

    for (auto& token : mytokenizer) {
        std::cout << token << '\n';
    }
}

