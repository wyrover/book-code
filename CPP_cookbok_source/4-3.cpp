#include <string>
#include <iostream>

using namespace std;

// The generic approach for trimming single
// characters from a string
template<typename T>
void rtrim(basic_string<T>& s, T c)
{
    if (s.empty())
        return;

    typename basic_string<T>::iterator p;

    for (p = s.end(); p != s.begin() && *--p == c;);

    if (*p != c)
        p++;

    s.erase(p, s.end());
}

int main()
{
    string s = "Great!!!!";
    wstring ws = L"Super!!!!";
    rtrim(s, '!');
    rtrim(ws, L'!');
    cout << s << '\n';
    wcout << ws << L'\n';
}
