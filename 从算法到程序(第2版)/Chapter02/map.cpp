#include <map>
#include <iostream>
using namespace std;

int main()
{
    map <string, float, less<string> > c;
    c.insert(make_pair("Cafe", 7.75));
    c.insert(make_pair("Banana", 1.72));
    c.insert(make_pair("Piza", 30.69));
    c["Wine"] = 15.66;
    map <string, float> :: iterator pos;

    for (pos = c.begin(); pos != c.end(); pos++)
        cout << pos->first << " " << pos->second << endl;

    c.clear();
    return 0;
}

