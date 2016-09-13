#include <iostream>
#include <string>
#include <regex>

using namespace std;

int main()
{
    regex reg("([\\w]+)");
    const string format("$1\n");

    while (true) {
        cout << "Enter a string to split over multiple lines (q=quit): ";
        string str;

        if (!getline(cin, str) || str == "q")
            break;

        cout << regex_replace(str, reg, format, regex_constants::format_no_copy) << endl;
    }

    return 0;
}
