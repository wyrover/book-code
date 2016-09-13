#include <algorithm>
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
    vector<int> myVector;
    populateContainer(myVector);
    cout << "The vector contents are:" << endl;

    for (const auto& i : myVector) {
        cout << i << " ";
    }

    cout << endl;
    transform(begin(myVector), end(myVector), begin(myVector),
    [](int i) {
        return i + 100;
    });
    cout << "The vector contents are:" << endl;

    for (const auto& i : myVector) {
        cout << i << " ";
    }

    cout << endl;
    return 0;
}
