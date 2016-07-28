// Program 10.9 Variations on a single integer
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>

int main(void)
{
    int k = 678;
    // Display formats as heading then display the values
    printf_s("%%d   %%o   %%x   %%X\n");
    printf_s("%d  %o  %x  %X\n", k, k, k, k);
    printf_s("\n|%%8d       |%%-8d      |%%+8d      |%%08d      |%%-+8d     |\n");
    printf_s("|%8d  |%-8d  |%-+8d  |%08d  |%-+8d  |\n", k, k, k, k, k);
    return 0;
}

