#include <string>
#include <iostream>

using namespace std;

int nthSubstr(int n, const string& s,
              const string& p)
{
    string::size_type i = s.find(p);     // Find the first occurrence
    int j;

    for (j = 1; j < n && i != string::npos; ++j)
        i = s.find(p, i + 1); // Find the next occurrence

    if (j == n)
        return (i);
    else
        return (-1);
}

int main()
{
    string s = "the wind, the sea, the sky, the trees";
    string p = "the";
    cout << nthSubstr(1, s, p) << '\n';
    cout << nthSubstr(2, s, p) << '\n';
    cout << nthSubstr(5, s, p) << '\n';
}
