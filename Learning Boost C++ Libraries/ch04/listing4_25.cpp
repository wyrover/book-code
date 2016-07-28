#include "qstring_token_generator.h"
#include <boost/tokenizer.hpp>
#include <iostream>

int main()
{
    std::string input = "I'm taking a train from Frankfurt "
                        "(am Main) to Frankfurt (an der Oder)";
    bool skipEmpty = true;
    qstring_token_generator qsep('(', ')', '\\', skipEmpty);
    typedef boost::tokenizer<qstring_token_generator> qtokenizer;
    qtokenizer tokenizer(input, qsep);
    unsigned int n = 0;

    for (auto& token : tokenizer) {
        std::cout << ++n << ':' << token << '\n';
    }
}
