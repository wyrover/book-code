#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>
#include <limits>
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
    populateContainer(vec);
    int low = numeric_limits<short>::min();  // = -32768
    int up = numeric_limits<short>::max();   // = 32767
    replace_if(begin(vec), end(vec), [low](int i) {
        return i < low;
    }, low);
    replace_if(begin(vec), end(vec), [up](int i) {
        return i > up;
    }, up);

    for (const auto& i : vec) {
        cout << i << " ";
    }

    cout << endl;
    return 0;
}
