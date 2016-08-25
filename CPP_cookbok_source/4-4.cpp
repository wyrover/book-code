#include <string>
#include <iostream>
#include <cctype>
#include <cwctype>

using namespace std;

template<typename T, typename F>
void rtrimws(basic_string<T>& s, F f)
{
    if (s.empty())
        return;

    typename basic_string<T>::iterator p;

    for (p = s.end(); p != s.begin() && f(*--p););

    if (!f(*p))
        p++;

    s.erase(p, s.end());
}

// Overloads to make cleaner calling for client code
void rtrimws(string& s)
{
    rtrimws(s, isspace);
}

void rtrimws(wstring& ws)
{
    rtrimws(ws, iswspace);
}

int main()
{
    string s = "zing ";
    wstring ws = L"zong ";
    rtrimws(s);
    rtrimws(ws);
    cout << s << "|\n";
    wcout << ws << L"|\n";
}
