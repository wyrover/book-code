#include <cstddef>
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
    vector<int> vec1, vec2;
    populateContainer(vec1);
    size_t cnt = 0;
    cout << "Enter number of elements you want to copy: ";
    cin >> cnt;
    cnt = min(cnt, vec1.size());
    vec2.resize(cnt);
    copy_n(cbegin(vec1), cnt, begin(vec2));

    for (const auto& i : vec2) {
        cout << i << " ";
    }

    cout << endl;
    return 0;
}
