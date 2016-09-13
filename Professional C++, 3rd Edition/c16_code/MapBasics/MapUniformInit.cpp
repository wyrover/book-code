#include <iostream>
#include <map>
#include <string>

using namespace std;

int main()
{
    map<string, int> m = {
        { "Marc G.", 123 },
        { "Warren B.", 456 },
        { "Peter V.W.", 666 }
    };

    for (const auto& p : m) {
        cout << p.first << " = " << p.second << endl;
    }

    return 0;
}
