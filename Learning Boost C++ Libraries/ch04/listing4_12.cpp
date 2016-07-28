#include <string>
#include <boost/algorithm/string/find.hpp>

struct EqualsShift {
    EqualsShift(unsigned int n) : shift(n) {}

    bool operator()(char input, char search) const
    {
        int disp = tolower(input) - 'a' - shift;
        return tolower(search) == (disp >= 0) ? 'a' : 'z' + disp;
    }

private:
    unsigned long shift;
};

int main()
{
    // encoded ... How little is too little
    std::string encoded = "Lsa pmxxpi mw xss pmxxpi";
    std::string realWord = "little";
    auto ret = boost::find(encoded,
                           boost::first_finder(realWord,
                                   EqualsShift(4)));
}
