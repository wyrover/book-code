#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>

using namespace std;

int main()
{
    string str1 = "750";
    string str2 = "2.71";
    string str3 = "0x7FFF";

    try {
        cout << boost::lexical_cast<int>(str1) << endl;
        cout << boost::lexical_cast<double>(str2) << endl;
        cout << boost::lexical_cast<int>(str3) << endl;
    } catch (boost::bad_lexical_cast& e) {
        cerr << "Bad cast: " << e.what() << endl;
    }
}
