#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

int main()
{
    string s1 = " leading spaces?";
    trim_left(s1); // Trim the original
    string s2 = trim_left_copy(s1); // Trim, but leave original intact
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    s1 = "YYYYboostXXX";
    s2 = trim_copy_if(s1, is_any_of("XY")); // Use a predicate to
    trim_if(s1, is_any_of("XY"));
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    s1 = "1234 numbers 9876";
    s2 = trim_copy_if(s1, is_digit());
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    // Nest calls to trim functions if you like
    s1 = " ****Trim!*** ";
    s2 = trim_copy_if(trim_copy(s1), is_any_of("*"));
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
}
