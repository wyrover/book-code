#include <iostream>
#include <istream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
#include "utils.h" // For printContainer( ): see 7.10

using namespace std;

int main()
{
    cout << "Enter a series of strings: ";
    istream_iterator<string> start(cin);
    istream_iterator<string> end; // This creates a "marker"
    vector<string> v(start, end);
    // Rearrange the elements in v so that those that are less
    // than "foo" occur before the rest.
    vector<string>::iterator p =
        partition(v.begin(), v.end(),
                  bind2nd(less<string>(), "foo"));
    printContainer(v);
    cout << "*p = " << *p << endl;
}
