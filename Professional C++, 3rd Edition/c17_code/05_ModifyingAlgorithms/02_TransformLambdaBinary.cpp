#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

// Function template to populate a container of ints.
// The container must support push_back().
template<typename Container>
void populateContainer(Container& cont)
{
    int num;

    while (true) {
        cout << "Enter a number (0 to quit): ";
        cin >> num;

        if (num == 0) {
            break;
        }

        cont.push_back(num);
    }
}

int main()
{
    vector<int> vec1;
    cout << "Vector1:" << endl;
    populateContainer(vec1);
    cout << "Vector2:" << endl;
    vector<int> vec2;
    populateContainer(vec2);

    if (vec2.size() < vec1.size()) {
        cout << "Vector2 should be at least the same size as vector1." << endl;
        return 1;
    }

    // Create a lambda to print a container
    auto printContainer = [](const auto & container) {
        for (auto& i : container) {
            cout << i << " ";
        }

        cout << endl;
    };
    cout << "Vector1: ";
    printContainer(vec1);
    cout << "Vector2: ";
    printContainer(vec2);
    transform(begin(vec1), end(vec1), begin(vec2), begin(vec1),
    [](int a, int b) {
        return a + b;
    });
    cout << "Vector1: ";
    printContainer(vec1);
    cout << "Vector2: ";
    printContainer(vec2);
    return 0;
}
