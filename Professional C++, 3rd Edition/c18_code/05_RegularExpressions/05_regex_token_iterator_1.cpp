#include <iostream>
#include <string>
#include <regex>

using namespace std;

int main()
{
    regex reg("[\\w]+");

    while (true) {
        cout << "Enter a string to split (q=quit): ";
        string str;

        if (!getline(cin, str) || str == "q")
            break;

        const sregex_token_iterator end;

        for (sregex_token_iterator iter(cbegin(str), cend(str), reg);
             iter != end; ++iter) {
            cout << "\"" << *iter << "\"" << endl;
        }
    }

    return 0;
}