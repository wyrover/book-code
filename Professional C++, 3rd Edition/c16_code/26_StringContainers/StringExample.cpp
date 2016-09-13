#include <string>
#include <iostream>
using namespace std;

int main()
{
    string str1;
    str1.insert(cend(str1), 'h');
    str1.insert(cend(str1), 'e');
    str1.push_back('l');
    str1.push_back('l');
    str1.push_back('o');

    for (const auto& letter : str1) {
        cout << letter;
    }

    cout << endl;

    for (auto it = cbegin(str1); it != cend(str1); ++it) {
        cout << *it;
    }

    cout << endl;
    return 0;
}
