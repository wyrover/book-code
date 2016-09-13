#include <functional>
#include <algorithm>
#include <cctype>
#include <string>
#include <iostream>
using namespace std;

class myIsDigit : public unary_function<char, bool>
{
public:
    bool operator()(char c) const
    {
        return ::isdigit(c) != 0;
    }
};

bool isNumber(const string& str)
{
    auto endIter = end(str);
    auto it = find_if(begin(str), endIter, not1(myIsDigit()));
    return (it == endIter);
}

int main()
{
    cout << isNumber("12345") << endl;
    cout << isNumber("hello") << endl;
    cout << isNumber("1234a") << endl;
    return 0;
}
