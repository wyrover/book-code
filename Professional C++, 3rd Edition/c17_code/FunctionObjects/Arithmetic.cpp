#include <iostream>
#include <numeric>
#include <vector>
#include <cmath>
#include <functional>
using namespace std;

double geometricMean(const vector<int>& nums)
{
    double mult = accumulate(cbegin(nums), cend(nums), 1, multiplies<int>());
    return pow(mult, 1.0 / nums.size());
}

// Using C++14 transparent operator functor
double geometricMeanTransparent(const vector<int>& nums)
{
    double mult = accumulate(cbegin(nums), cend(nums), 1, multiplies<>());
    return pow(mult, 1.0 / nums.size());
}

int main()
{
    plus<int> myPlus;
    int res = myPlus(4, 5);
    cout << res << endl;
    vector<int> myVector;
    int num;

    while (true) {
        cout << "Enter a test score to add (0 to stop): ";
        cin >> num;

        if (num == 0) {
            break;
        }

        myVector.push_back(num);
    }

    cout << "The geometric mean is " << geometricMean(myVector) << endl;
    cout << "The geometric mean is (transparent function object) "
         << geometricMeanTransparent(myVector) << endl;
    return 0;
}
