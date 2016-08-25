#include <list>
#include <algorithm>
#include <iostream>

using namespace std;

void write(const string& s)
{
    cout << s << '\n';
}

int main()
{
    list<string> lst;
    string s = "knife";
    lst.push_front(s);
    s = "fork";
    lst.push_back(s);
    s = "spoon";
    lst.push_back(s);
    // A list doesn't have random access, so
    // use for_each( ) instead
    for_each(lst.begin(), lst.end(), write);
}
