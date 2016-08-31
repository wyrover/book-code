#include <iostream>
#include <string>
#include <tchar.h>



using namespace std;

string wstring_to_string(const wstring &ws)
{
    string curLocale = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, "chs");
    const wchar_t * _Source = ws.c_str();
    size_t _Dsize = 2 * ws.size() + 1;
    char *_Dest = new char[_Dsize];
    memset(_Dest, 0, _Dsize);
    wcstombs(_Dest, _Source, _Dsize);
    string result = _Dest;
    delete [] _Dest;
    setlocale(LC_ALL, curLocale.c_str());
    return result;
}

wstring string_to_wstring(const string &s)
{
    string curLocale = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, "chs");
    const char *_Source = s.c_str();
    size_t _Dsize = s.size() + 1;
    wchar_t * _Dest = new wchar_t[_Dsize];
    wmemset(_Dest, 0, _Dsize);
    mbstowcs(_Dest, _Source, _Dsize);
    wstring result = _Dest;
    delete [] _Dest;
    setlocale(LC_ALL, curLocale.c_str());
    return result;
}
int _tmain(int argc, _TCHAR *argv [])
{
    wstring ws1, ws2 = L"abcd1234,!@#$和风熏柳，花香醉人，正是南国春光漫烂季节。";
    string s;
    s = wstring_to_string(ws2);
    ws1 = string_to_wstring(s);
    setlocale(LC_ALL, "chs");
    wprintf(L"原wstring串=%s\n", ws2.c_str());
    printf("转换为string串=%s\n", s.c_str());
    wprintf(L"转换为wstring串=%s\n", ws1.c_str());
    getchar();
    return 0;
}
