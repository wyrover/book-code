// Program 8.1 Scoping out scope
#include <stdio.h>

int main(void)
{
    int count1 = 1;                               // Declared in outer block

    do {
        int count2 = 0;                             // Declared in inner block
        ++count2;
        printf("count1 = %d     count2 = %d\n", count1, count2);
    } while (++count1 <= 5);

    // count2 no longer exists
    printf("count1 = %d\n", count1);
    return 0;
}
