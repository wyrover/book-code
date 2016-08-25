#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char str[] = "abcd≤‚ ‘";
    wchar_t wstr[] = L"abcd≤‚ ‘";
    wcstombs(str, wstr, sizeof(str));
    printf("%s\n", str);

    getchar();
    
    return 0;
}
