#include <iostream>
#include <numeric>
#include <vector>
#include <cmath>
#include <functional>
using namespace std;

bool allTrue(const vector<bool>& flags)
{
    return accumulate(begin(flags), end(flags), true, logical_and<>());         // C++14 transparent function object
    //return accumulate(begin(flags), end(flags), true, logical_and<bool>());   // Pre-C++14
}

bool anyTrue(const vector<bool>& flags)
{
    return accumulate(begin(flags), end(flags), false, logical_or<>());         // C++14 transparent function object
    //return accumulate(begin(flags), end(flags), false, logical_or<bool>());   // Pre-C++14
}

int main()
{
    vector<bool> myVector;
    int num;

    while (true) {
        cout << "Enter a value 0 or 1 to add (-1 to stop): ";
        cin >> num;

        if (num == -1) {
            break;
        }

        myVector.push_back(num == 1);
    }

    cout << "allTrue? " << (allTrue(myVector) ? "yes" : "no") << endl;
    cout << "anyTrue? " << (anyTrue(myVector) ? "yes" : "no") << endl;
    return 0;
}
