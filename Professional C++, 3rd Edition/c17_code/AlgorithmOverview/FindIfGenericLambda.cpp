#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;

int main()
{
    vector<int> ints{ 11, 55, 101, 200 };
    vector<double> doubles{ 11.1, 55.5, 200.2 };
    // Define a generic lambda to find values > 100.
    auto isGreaterThan100 = [](auto i) {
        return i > 100;
    };
    // Use the generic lambda with the vector of integers.
    auto it1 = find_if(cbegin(ints), cend(ints), isGreaterThan100);

    if (it1 != cend(ints)) {
        cout << "Found a value > 100: " << *it1 << endl;
    }

    // Use exactly the same generic lambda with the vector of doubles.
    auto it2 = find_if(cbegin(doubles), cend(doubles), isGreaterThan100);

    if (it2 != cend(doubles)) {
        cout << "Found a value > 100: " << *it2 << endl;
    }

    return 0;
}
