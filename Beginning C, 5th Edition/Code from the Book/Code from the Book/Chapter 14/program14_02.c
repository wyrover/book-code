// Program 14.2 Finding occurrences of one wide character string in another
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>

#define TEXT_SIZE   100
#define SUBSTR_SIZE  40

wchar_t *wstr_towupper(wchar_t *wstr, size_t size);        // Wide string to uppercase

int main(void)
{
    wchar_t text[TEXT_SIZE];                                 // Input buffer for string to be searched
    wchar_t substr[SUBSTR_SIZE];                             // Input buffer for string sought
    wprintf_s(L"Enter the string to be searched (less than %d characters):\n", TEXT_SIZE);
    fgetws(text, TEXT_SIZE, stdin);
    wprintf_s(L"\nEnter the string sought (less than %d characters):\n", SUBSTR_SIZE);
    fgetws(substr, SUBSTR_SIZE, stdin);
    // Overwrite the newline character in each string
    int textlen = wcsnlen_s(text, sizeof(text) / sizeof(wchar_t));
    int substrlen = wcsnlen_s(substr, sizeof(substr) / sizeof(wchar_t));
    text[--textlen] = L'\0';
    substr[--substrlen] = L'\0';
    fwprintf_s(stdout, L"\nFirst string entered:\n%ls\n", text);
    fwprintf_s(stdout, L"Second string entered:\n%ls\n", substr);
    // Convert both strings to uppercase
    wstr_towupper(text, sizeof(text) / sizeof(wchar_t));
    wstr_towupper(substr, sizeof(substr) / sizeof(wchar_t));
    // Count the appearances of substr in text
    wchar_t *pwstr = text;
    int count = 0;

    while ((pwstr < text + textlen - substrlen) && (pwstr = wcsstr(pwstr, substr))) {
        ++count;
        pwstr += substrlen;
    }

    wprintf_s(L"The second string %ls found in the first%ls",
              count ? L"was" : L"was not", count ? L" " : L".\n");

    if (count)
        wprintf_s(L"%d times.\n", count);

    return 0;
}

// Convert a wide string to uppercase
wchar_t *wstr_towupper(wchar_t *wstr, size_t size)
{
    for (size_t i = 0 ; i < wcsnlen_s(wstr, size) ; ++i)
        wstr[i] = towupper(wstr[i]);

    return wstr;
}
