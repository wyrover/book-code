#include <iostream>
#include <string>
#include <cctype>
#include <cwctype>
#include <stdexcept>

using namespace std;

void toUpper(basic_string<char>& s)
{
    for (basic_string<char>::iterator p = s.begin();
         p != s.end(); ++p) {
        *p = toupper(*p); // toupper is for char
    }
}

void toUpper(basic_string<wchar_t>& s)
{
    for (basic_string<wchar_t>::iterator p = s.begin();
         p != s.end(); ++p) {
        *p = towupper(*p); // towupper is for wchar_t
    }
}

void toLower(basic_string<char>& s)
{
    for (basic_string<char>::iterator p = s.begin();
         p != s.end(); ++p) {
        *p = tolower(*p);
    }
}

void toLower(basic_string<wchar_t>& s)
{
    for (basic_string<wchar_t>::iterator p = s.begin();
         p != s.end(); ++p) {
        *p = towlower(*p);
    }
}

int main()
{
    string s = "shazam";
    wstring ws = L"wham";
    toUpper(s);
    toUpper(ws);
    cout << "s =  " << s << endl;
    wcout << "ws = " << ws << endl;
    toLower(s);
    toLower(ws);
    cout << "s =  " << s << endl;
    wcout << "ws = " << ws << endl;
}
