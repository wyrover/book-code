#include <iostream>

using namespace std;

int main()
{
    const char* s1 = u8R"(Raw UTF-8 encoded string literal)";
    const wchar_t* s2 = LR"(Raw wide string literal)";
    const char16_t* s3 = uR"(Raw char16_t string literal)";
    const char32_t* s4 = UR"(Raw char32_t string literal)";
    cout << s1 << endl;
    wcout << s2 << endl;
    // Can't stream s3 and s4, because streams don't support char16_t and char32_t.
    const char* formula = u8"\u03C0 r\u00B2";
    cout << formula << endl;
    return 0;
}
