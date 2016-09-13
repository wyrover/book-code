#include <algorithm>
#include <vector>
#include <iostream>
#include <functional>
using namespace std;

bool perfectScore(int num)
{
    return (num >= 100);
}

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
    // Using not1
    function<bool(int)> f = perfectScore;
    auto it = find_if(begin(myVector), endIter, not1(f));

    // Using C++11 Lambda:
    //auto it = find_if(begin(myVector), endIter, [](int i){ return i < 100; });

    if (it == endIter) {
        cout << "All perfect scores" << endl;
    } else {
        cout << "Found a \"less-than-perfect\" score of " << *it << endl;
    }

    return 0;
}
