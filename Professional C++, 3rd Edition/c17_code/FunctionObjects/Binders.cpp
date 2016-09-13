#include <algorithm>
#include <vector>
#include <iostream>
#include <functional>
using namespace std;

int main()
{
    int num;
    vector<int> myVector;

    while (true) {
        cout << "Enter a test score to add (0 to stop): ";
        cin >> num;

        if (num == 0) {
            break;
        }

        myVector.push_back(num);
    }

    auto endIter = end(myVector);
    // C++14 solution using std::bind and transparent function object
    auto it = find_if(begin(myVector), endIter,
                      bind(greater_equal<>(), placeholders::_1, 100));

    // C++11 solution using std::bind
    //auto it = find_if(begin(myVector), endIter,
    //  bind(greater_equal<int>(), placeholders::_1, 100));

    // Recommended C++11/C++14 solution with lambda
    //auto it = find_if(begin(myVector), endIter, [](int i){ return i >= 100; });

    // Deprecated pre-C++11 solution using bind2nd
    //vector<int>::iterator it = find_if(myVector.begin(), end,
    //                  bind2nd(greater_equal<int>(), 100));

    if (it == endIter) {
        cout << "No perfect scores" << endl;
    } else {
        cout << "Found a \"perfect\" score of " << *it << endl;
    }

    return 0;
}
