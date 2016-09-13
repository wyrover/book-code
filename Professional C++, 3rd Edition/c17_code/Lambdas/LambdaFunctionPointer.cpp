#include <string>
#include <iostream>

using namespace std;

int main()
{
    auto fn = [](const string & str) {
        return "Hello from " + str;
    };
    cout << fn("call 1") << endl;
    cout << fn("call 2") << endl;
    return 0;
}
