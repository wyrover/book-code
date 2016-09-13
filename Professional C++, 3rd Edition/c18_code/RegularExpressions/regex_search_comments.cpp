#include <iostream>
#include <string>
#include <regex>

using namespace std;

int main()
{
    regex r("//\\s*(.+)$");

    while (true) {
        cout << "Enter a string with optional code comments (q=quit): ";
        string str;

        if (!getline(cin, str) || str == "q")
            break;

        smatch m;

        if (regex_search(str, m, r))
            cout << "  Found comment '" << m[1] << "'" << endl;
        else
            cout << "  No comment found!" << endl;
    }

    return 0;
}
