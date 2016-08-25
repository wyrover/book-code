#include <iostream>
#include <boost/lexical_cast.hpp>

using namespace std;
using boost::lexical_cast;
using boost::bad_lexical_cast;

template<typename T>
bool isValid(const string& num)
{
    bool res = true;

    try {
        T tmp = lexical_cast<T>(num);
    } catch (bad_lexical_cast &e) {
        res = false;
    }

    return (res);
}

void test(const string& s)
{
    if (isValid<int>(s))
        cout << s << " is a valid integer." << endl;
    else
        cout << s << " is NOT a valid integer." << endl;

    if (isValid<double>(s))
        cout << s << " is a valid double." << endl;
    else
        cout << s << " is NOT a valid double." << endl;

    if (isValid<float>(s))
        cout << s << " is a valid float." << endl;
    else
        cout << s << " is NOT a valid float." << endl;
}

int main()
{
    test("12345");
    test("1.23456");
    test("-1.23456");
    test(" - 1.23456");
    test("+1.23456");
    test(" 1.23456 ");
    test("asdf");
}
