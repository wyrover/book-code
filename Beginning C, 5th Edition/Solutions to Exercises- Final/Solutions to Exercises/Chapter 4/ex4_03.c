/*Exercise 4.3 Displaying printable characters plus whitspace names */
#include <stdio.h>
#include <ctype.h>

int main(void)
{
    char ch = 0;                       /* Character code value */

    for (int i = 0 ; i < 128 ; ++i) {
        ch = (char)i;

        if (i % 2 == 0)
            printf("\n");

        printf("  %4d", ch);

        if (isgraph(ch))
            printf("               %c", ch);
        else {
            switch (ch) {
            case '\n':
                printf("         newline", ch);
                break;

            case ' ':
                printf("           space", ch);
                break;

            case '\t':
                printf("  horizontal tab", ch);
                break;

            case '\v':
                printf("    vertical tab", ch);
                break;

            case '\f':
                printf("       form feed", ch);
                break;

            default:
                printf("                ");
                break;
            }
        }
    }

    printf("\n");
    return 0;
}

