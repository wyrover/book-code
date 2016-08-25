#include <string>
#include <iostream>

using namespace std;

template<typename T>
int nthSubstrg(int n, const basic_string<T>& s,
               const basic_string<T>& p,
               bool repeats = false)
{
    string::size_type i = s.find(p);
    string::size_type adv = (repeats) ? 1 : p.length();
    int j;

    for (j = 1; j < n && i != basic_string<T>::npos; ++j)
        i = s.find(p, i + adv);

    if (j == n)
        return (i);
    else
        return (-1);
}

int main()
{
    string s = "AGATGCCATATATATACGATATCCTTA";
    string p = "ATAT";
    cout << p << " as non-repeating occurs at "
         << nthSubstrg(3, s, p) << '\n';
    cout << p << " as repeating occurs at "
         << nthSubstrg(3, s, p, true) << '\n';
}
