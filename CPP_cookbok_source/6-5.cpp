#include <iostream>
#include <list>
#include <string>
#include <algorithm>

using namespace std;

// A simple functor for printing
template<typename T>
struct printer {
    void operator()(const T& s)
    {
        cout << s << '\n';
    }
};

bool inline even(int n)
{
    return (n % 2 == 0);
}

printer<string> strPrinter;
printer<int>    intPrinter;

int main()
{
    list<string> lstOne;
    list<string> lstTwo;
    lstOne.push_back("Red");
    lstOne.push_back("Green");
    lstOne.push_back("Blue");
    lstTwo.push_front("Orange");
    lstTwo.push_front("Yellow");
    lstTwo.push_front("Fuschia");
    for_each(lstOne.begin(),  // Print each element in the list
             lstOne.end(),    // with a custom functor, print
             strPrinter);
    lstOne.sort();            // list has a member for sorting
    lstTwo.sort();
    lstOne.merge(lstTwo);     // Merge the two lists and print
    for_each(lstOne.begin(),  // the results (the lists must be
             lstOne.end(),    // sorted before merging)
             strPrinter);
    list<int> intLst;
    intLst.push_back(0);
    intLst.push_back(1);
    intLst.push_back(2);
    intLst.push_back(3);
    intLst.push_back(4);
    // Remove all values greater than 2
    intLst.remove_if(bind2nd(greater<int>(), 2));
    for_each(intLst.begin(),
             intLst.end(),
             intPrinter);
    // Or, remove all even values
    intLst.remove_if(even);
}
