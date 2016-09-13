#include <functional>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

void removeEmptyStrings(vector<string>& strings)
{
    auto it = remove_if(begin(strings), end(strings),
    [](const string & str) {
        return str.empty();
    });
    // erase the removed elements
    strings.erase(it, end(strings));
}

int main()
{
    vector<string> myVector = {"", "one", "", "two", "three", "four"};

    for (auto& str : myVector) {
        cout << "\"" << str << "\"  ";
    }

    cout << endl;
    removeEmptyStrings(myVector);

    for (auto& str : myVector) {
        cout << "\"" << str << "\"  ";
    }

    cout << endl;
    return 0;
}
