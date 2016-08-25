#include <iostream>
#include <list>
#include <algorithm>
#include <string>

using namespace std;

static const int ARRAY_SIZE = 5;

template<typename T,
         typename FwdIter>
FwdIter fixOutliersUBound(FwdIter p1,
                          FwdIter p2,
                          const T& oldVal,
                          const T& newVal)
{
    for (; p1 != p2; ++p1) {
        if (greater<T>(*p1, oldVal)) {
            *p1 = newVal;
        }
    }
}

int main()
{
    list<string> lstStr;
    lstStr.push_back("Please");
    lstStr.push_back("leave");
    lstStr.push_back("a");
    lstStr.push_back("message");

    // Create an iterator for stepping through the list
    for (list<string>::iterator p = lstStr.begin();
         p != lstStr.end(); ++p) {
        cout << *p << endl;
    }

    // Or I can use a reverse_iterator to go from the end
    // to the beginning.  rbegin returns a reverse_iterator
    // to the last element and rend returns a reverse_iterator
    // to one-before-the-first.
    for (list<string>::reverse_iterator p = lstStr.rbegin();
         p != lstStr.rend(); ++p) {
        cout << *p << endl;
    }

    // Iterating through a range
    string arrStr[ARRAY_SIZE] = {"My", "cup", "cup", "runneth", "over"};

    for (string* p = &arrStr[0];
         p != &arrStr[ARRAY_SIZE]; ++p) {
        cout << *p << endl;
    }

    // Using standard algorithms with a standard sequence
    list<string> lstStrDest;
    unique_copy(&arrStr[0], &arrStr[ARRAY_SIZE],
                back_inserter(lstStrDest));
}
