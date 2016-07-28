// Program 10.4 Reading hexadecimal and octal values
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>

int main(void)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int n = 0;
    printf("Enter three integer values:");
    n = scanf_s(" %d %x %o", &i , &j, &k);
    printf("\nOutput:\n");
    printf("%d values read.\n", n);
    printf("i = %d   j = %d   k = %d\n", i, j, k);
//  printf("i = %x   j = %X   k = %d\n", i, j, k );     // Alternate
    return 0;
}
