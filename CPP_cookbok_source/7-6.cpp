#include <iostream>
#include <istream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <iterator>
#include "utils.h" // For printContainer( ): see 7.10

using namespace std;

int main()
{
    cout << "Enter a series of strings: ";
    istream_iterator<string> start(cin);
    istream_iterator<string> end; // This creates a "marker"
    vector<string> v(start, end);
    // The sort standard algorithm will sort elements in a range.  It
    // requires a random-access iterator, so it works for a vector.
    sort(v.begin(), v.end());
    printContainer(v);
    random_shuffle(v.begin(), v.end());   // See 7.2
    string* arr = new string[v.size()];
    // Copy the elements into the array
    copy(v.begin(), v.end(), &arr[0]);
    // Sort works on any kind of range, so long as its arguments
    // behave like random-access iterators.
    sort(&arr[0], &arr[v.size()]);
    printRange(&arr[0], &arr[v.size()]);
    // Create a list with the same elements
    list<string> lst(v.begin(), v.end());
    lst.sort();  // The standalone version of sort won't work; you have
    // to use list::sort.  Note, consequently, that you
    // can't sort only parts of a list.
    printContainer(lst);
}
