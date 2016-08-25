#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    // Create some strings with Unicode characters
    wstring ws1 = L"Infinity: \u221E";
    wstring ws2 = L"Euro: _";
    wchar_t w[] = L"Infinity: \u221E";
    wofstream out("tmp\\unicode.txt");
    out << ws2 << endl;
    wcout << ws2 << endl;
}