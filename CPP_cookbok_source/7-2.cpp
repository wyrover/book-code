#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include <functional>
#include "utils.h" // For printContainer( ): see 7.10

using namespace std;

int main()
{
    list<string> lstStr;
    lstStr.push_back("On");
    lstStr.push_back("a");
    lstStr.push_back("cloudy");
    lstStr.push_back("cloudy");
    lstStr.push_back("day");
    list<string>::iterator p;
    // Find what you want with find
    p = find(lstStr.begin(), lstStr.end(), "day");
    p = lstStr.erase(p); // Now p points to the last element
    // Or, to erase all occurrences of something, use remove
    lstStr.erase(remove(lstStr.begin(), lstStr.end(), "cloudy"),
                 lstStr.end());
    printContainer(lstStr); // See 7.10
}
