#include <algorithm>
#include <vector>
#include <iostream>
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
    vector<int> vec;
    cout << "Enter values:" << endl;
    populateContainer(vec);
    sort(begin(vec), end(vec));

    for (const auto& i : vec) {
        cout << i << " ";
    }

    cout << endl;
    return 0;
}
