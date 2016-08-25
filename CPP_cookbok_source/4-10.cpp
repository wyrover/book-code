#include <string>
#include <vector>
#include <functional>
#include <iostream>

using namespace std;

void split(const string& s, char c,
           vector<string>& v)
{
    string::size_type i = 0;
    string::size_type j = s.find(c);

    while (j != string::npos) {
        v.push_back(s.substr(i, j - i));
        i = ++j;
        j = s.find(c, j);

        if (j == string::npos)
            v.push_back(s.substr(i, s.length()));
    }
}

int main()
{
    vector<string> v;
    string s = "Account Name|Address 1|Address 2|City";
    split(s, '|', v);

    for (int i = 0; i < v.size(); ++i) {
        cout << v[i] << '\n';
    }
}
