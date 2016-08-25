#include <iostream>
#include <sstream>
#include <string>

using namespace std;

double sciToDub(const string& str)
{
    stringstream ss(str);
    double d = 0;
    ss >> d;

    if (ss.fail()) {
        string s = "Unable to format ";
        s += str;
        s += " as a number!";
        throw (s);
    }

    return (d);
}

int main()
{
    try {
        cout << sciToDub("1.234e5") << endl;
        cout << sciToDub("6.02e-2") << endl;
        cout << sciToDub("asdf") << endl;
    } catch (string& e) {
        cerr << "Whoops: " << e << endl;
    }
}
