#include <iostream>
#include <set>
#include <string>

using namespace std;

int main()
{
    set<string> setStr;
    string s = "Bill";
    setStr.insert(s);
    s = "Steve";
    setStr.insert(s);
    s = "Randy";
    setStr.insert(s);
    s = "Howard";
    setStr.insert(s);

    for (set<string>::const_iterator p = setStr.begin();
         p != setStr.end(); ++p)
        cout << *p << endl;
}
