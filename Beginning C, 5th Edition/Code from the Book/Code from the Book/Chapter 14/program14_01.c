// Program 14.1 Classifying wide characters
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>

int main(void)
{
    wchar_t ch = 0;                                   // Stores a character
    fwprintf_s(stdout, L"Enter a character: ");
    fwscanf_s(stdin, L" %lc", &ch, sizeof(ch));       // Read a non-whitespace character

    if (iswalnum(ch)) {                               // Is it a letter or a digit?
        if (iswdigit(ch))                               // Is it a digit?
            fwprintf_s(stdout, L"You entered the digit %lc\n", ch);
        else if (iswlower(ch))                          // Is a lowercase letter?
            fwprintf_s(stdout, L"You entered a lowercase %lc\n", towupper(ch));
        else
            fwprintf_s(stdout, L"You entered an uppercase %lc\n", towlower(ch));
    } else if (iswpunct(ch))                          // Is it punctuation?
        fwprintf_s(stdout, L"You entered the punctuation character %lc.\n", ch);
    else
        fwprintf_s(stdout, L"You entered %lc, but I don't know what it is!\n", ch);

    return 0;
}
