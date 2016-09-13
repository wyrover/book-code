#include <iostream>
#include <string>
#include <locale>

using namespace std;

int main()
{
    locale loc("");

    if (loc.name().find("en_US") == string::npos &&
        loc.name().find("United States") == string::npos) {
        wcout << L"Welcome non-U.S. English speaker!" << endl;
    } else {
        wcout << L"Welcome U.S. English speaker!" << endl;
    }

    return 0;
}
