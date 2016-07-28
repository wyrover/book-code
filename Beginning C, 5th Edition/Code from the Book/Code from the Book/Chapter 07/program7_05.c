// Program 7.5 Incrementing a pointer to an array
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <string.h>

int main(void)
{
    char multiple[] = "a string";
    char *p = multiple;

    for (int i = 0 ; i < strnlen_s(multiple, sizeof(multiple)) ; ++i)
        printf("multiple[%d] = %c  *(p+%d) = %c  &multiple[%d] = %p  p+%d = %p\n",
               i, multiple[i], i, *(p + i), i, &multiple[i], i, p + i);

    return 0;
}
