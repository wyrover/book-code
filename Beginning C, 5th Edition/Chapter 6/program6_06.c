// Program 6.6 A demonstration of seeking and finding
#include <stdio.h>
#include <string.h>
int main(void)
{
    char str1[] = "This string contains the holy grail.";
    char str2[] = "the holy grail";
    char str3[] = "the holy grill";

    // Search str1 for the occurrence of str2
    if (strstr(str1, str2))
        printf("\"%s\" was found in \"%s\"\n", str2, str1);
    else
        printf("\n\"%s\" was not found.", str2);

    // Search str1 for the occurrence of str3
    if (!strstr(str1, str3))
        printf("\"%s\" was not found.\n", str3);
    else
        printf("\nWe shouldn't get to here!");

    return 0;
}
