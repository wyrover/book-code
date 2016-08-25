#include <string>
#include <vector>
#include <iostream>

using namespace std;

void join(const vector<string>& v, char c, string& s)
{
    s.clear();

    for (vector<string>::const_iterator p = v.begin();
         p != v.end(); ++p) {
        s += *p;

        if (p != v.end() - 1)
            s += c;
    }
}

int main()
{
    vector<string> v;
    vector<string> v2;
    string s;
    v.push_back(string("fee"));
    v.push_back(string("fi"));
    v.push_back(string("foe"));
    v.push_back(string("fum"));
    join(v, '/', s);
    cout << s << '\n';
}
