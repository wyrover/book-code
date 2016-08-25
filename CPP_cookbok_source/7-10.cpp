#include <iostream>
#include <istream>
#include <iterator>
#include <string>
#include <functional>
#include <vector>
#include <list>
#include "utils.h" // For printContainer( ): see 7.10

using namespace std;

template<typename In, typename Out, typename UnPred>
Out copyIf(In first, In last, Out result, UnPred pred)
{
    for (; first != last; ++first)
        if (pred(*first))
            *result++ = *first;

    return (result);
}

int main()
{
    cout << "Enter a series of strings: ";
    istream_iterator<string> start(cin);
    istream_iterator<string> end; // This creates a "marker"
    vector<string> v(start, end);
    list<string> lst;
    copyIf(v.begin(), v.end(), back_inserter<list<string> >(lst),
           bind2nd(less<string>(), "cookie"));
    printContainer(lst);
}
