#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;

int main()
{
    int num;
    vector<int> myVector;

    while (true) {
        cout << "Enter a number to add (0 to stop): ";
        cin >> num;

        if (num == 0) {
            break;
        }

        myVector.push_back(num);
    }

    while (true) {
        cout << "Enter a number to lookup (0 to stop): ";
        cin >> num;

        if (num == 0) {
            break;
        }

        auto endIt = cend(myVector);
        auto it = find(cbegin(myVector), endIt, num);

        if (it == endIt) {
            cout << "Could not find " << num << endl;
        } else {
            cout << "Found " << *it << endl;
        }
    }

    return 0;
}
