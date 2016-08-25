#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <iterator>
#include "utils.h" // For printContainer( ): see 7.10

using namespace std;

int main()
{
    vector<string> v1, v2, v3;
    v1.push_back("a");
    v1.push_back("c");
    v1.push_back("e");
    v2.push_back("b");
    v2.push_back("d");
    v2.push_back("f");
    v3.reserve(v1.size() + v2.size() + 1);
    // Use a back_inserter from iterator to avoid having to put
    // a bunch of default objects in the container.  But this doesn't
    // mean you don't have to use reserve!
    merge(v1.begin(), v1.end(),
          v2.begin(), v2.end(),
          back_inserter<vector<string> >(v3));
    printContainer(v3);
    // Now make a mess
    random_shuffle(v3.begin(), v3.end());
    sort(v3.begin(), v3.begin() + v3.size() / 2);
    sort(v3.begin() + v3.size() / 2, v3.end());
    printContainer(v3);
    inplace_merge(v3.begin(), v3.begin() + 3, v3.end());
    printContainer(v3);
    // If you are using two lists, though, use list::merge instead.
    // As a general rule, blah blah...
    list<string> lstStr1, lstStr2;
    lstStr1.push_back("Frank");
    lstStr1.push_back("Rizzo");
    lstStr1.push_back("Bill");
    lstStr1.push_back("Cheetoh");
    lstStr2.push_back("Allie");
    lstStr2.push_back("McBeal");
    lstStr2.push_back("Slick");
    lstStr2.push_back("Willie");
    lstStr1.sort();   // Sort these or merge makes garbage!
    lstStr2.sort();
    lstStr1.merge(lstStr2); // Note that this only works with other
    // lists of the same type
    printContainer(lstStr1);
}
