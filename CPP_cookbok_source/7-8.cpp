#include <iostream>
#include <algorithm>
#include <string>
#include <set>
#include <iterator>
#include "utils.h" // For printContainer( ): see 7.10

using namespace std;

int main()
{
    cout << "Enter some strings: ";
    istream_iterator<string> start(cin);
    istream_iterator<string> end;
    set<string> s1(start, end);
    cin.clear();
    cout << "Enter some more strings: ";
    set<string> s2(++start, end);
    set<string> setUnion;
    set<string> setInter;
    set<string> setDiff;
    set_union(s1.begin(), s1.end(),
              s2.begin(), s2.end(),
              inserter(setUnion, setUnion.begin()));
    set_difference(s1.begin(), s1.end(),
                   s2.begin(), s2.end(),
                   inserter(setDiff, setDiff.begin()));
    set_intersection(s1.begin(), s1.end(),
                     s2.begin(), s2.end(),
                     inserter(setInter, setInter.begin()));
    cout << "Union:\n";
    printContainer(setUnion);
    cout << "Difference:\n";
    printContainer(setDiff);
    cout << "Intersection:\n";
    printContainer(setInter);
}
