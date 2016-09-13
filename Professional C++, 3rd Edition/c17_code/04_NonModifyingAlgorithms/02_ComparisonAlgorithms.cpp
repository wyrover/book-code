#include <algorithm>
#include <vector>
#include <list>
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
    vector<int> myVector;
    list<int> myList;
    cout << "Populate the vector:" << endl;
    populateContainer(myVector);
    cout << "Populate the list:" << endl;
    populateContainer(myList);

    // Compare the two containers
    if (myList.size() == myVector.size() &&
        equal(cbegin(myVector), cend(myVector), cbegin(myList))) {
        cout << "The two containers have equal elements" << endl;
    } else {
        if (myList.size() < myVector.size()) {
            cout << "Sorry, the list is not long enough." << endl;
            return 1;
        } else {
            // If the containers were not equal, find out why not
            auto miss = mismatch(cbegin(myVector), cend(myVector), cbegin(myList));
            cout << "The following initial elements are the same in the vector and the list:" << endl;

            for (auto i = cbegin(myVector); i != miss.first; ++i) {
                cout << *i << '\t';
            }

            cout << endl;
        }
    }

    // C++14: equal() and mismatch() accepting four iterators
    if (equal(cbegin(myVector), cend(myVector), cbegin(myList), cend(myList))) {
        cout << "The two containers have equal elements" << endl;
    } else {
        // If the containers were not equal, find out why not
        auto miss = mismatch(cbegin(myVector), cend(myVector),
                             cbegin(myList), cend(myList));
        cout << "The following initial elements are the same in "
             << "the vector and the list:" << endl;

        for (auto i = cbegin(myVector); i != miss.first; ++i) {
            cout << *i << '\t';
        }

        cout << endl;
    }

    // Now order them
    if (lexicographical_compare(cbegin(myVector), cend(myVector), cbegin(myList), cend(myList))) {
        cout << "The vector is lexicographically first." << endl;
    } else {
        cout << "The list is lexicographically first." << endl;
    }

    return 0;
}
