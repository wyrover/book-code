#include <iostream>
#include <string>
#include <list>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

int main()
{
    string s = "one,two,three,four";
    list<string> results;
    split(results, s, is_any_of(","));  // Note this is boost::split

    for (list<string>::const_iterator p = results.begin();
         p != results.end(); ++p) {
        cout << *p << endl;
    }
}
