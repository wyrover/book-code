// Exercise 4.2 Displaying printable characters
#include <stdio.h>
#include <ctype.h>

int main(void)
{
    char ch = 0;                       // Character code value

    for (int i = 0 ; i < 128 ; ++i) {
        ch = (char)i;

        if (i % 2 == 0)
            printf("\n");

        printf("  %4d    %c", ch, (isgraph(ch) ? ch : ' '));
    }

    printf("\n");
    return 0;
}

