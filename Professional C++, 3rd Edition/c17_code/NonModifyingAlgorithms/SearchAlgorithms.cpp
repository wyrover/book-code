#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main()
{
    // The list of elements to be searched
    vector<int> myVector = { 5, 6, 9, 8, 8, 3 };
    auto beginIter = cbegin(myVector);
    auto endIter = cend(myVector);
    // Find the first element that does not satisfy the given lambda expression
    auto it = find_if_not(beginIter, endIter, [](int i) {
        return i < 8;
    });

    if (it != endIter) {
        cout << "First element not < 8 is " << *it << endl;
    }

    // Find the min and max elements in the vector
    auto res = minmax_element(beginIter, endIter);
    cout << "min is " << *(res.first) << " and max is " << *(res.second) << endl;
    // Find the min and max elements in the vector
    it = min_element(beginIter, endIter);
    auto it2 = max_element(beginIter, endIter);
    cout << "min is " << *it << " and max is " << *it2 << endl;
    // Find the first pair of matching consecutive elements
    it = adjacent_find(beginIter, endIter);

    if (it != endIter) {
        cout << "Found two consecutive equal elements with value " << *it << endl;
    }

    // Find the first of two values
    vector<int> targets = { 8, 9 };
    it = find_first_of(beginIter, endIter, cbegin(targets), cend(targets));

    if (it != endIter) {
        cout << "Found one of 8 or 9: " << *it << endl;
    }

    // Find the first subsequence
    vector<int> sub = { 8, 3 };
    it = search(beginIter, endIter, cbegin(sub), cend(sub));

    if (it != endIter) {
        cout << "Found subsequence {8,3}" << endl;
    } else {
        cout << "Unable to find subsequence {8,3}" << endl;
    }

    // Find the last subsequence (which is the same as the first in this example)
    it2 = find_end(beginIter, endIter, cbegin(sub), cend(sub));

    if (it != it2) {
        cout << "Error: search and find_end found different subsequences "
             << "even though there is only one match." << endl;
    }

    // Find the first subsequence of two consecutive 8s
    it = search_n(beginIter, endIter, 2, 8);

    if (it != endIter) {
        cout << "Found two consecutive 8s" << endl;
    } else {
        cout << "Unable to find two consecutive 8s" << endl;
    }

    return 0;
}
