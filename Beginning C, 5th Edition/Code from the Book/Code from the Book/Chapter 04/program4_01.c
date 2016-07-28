// Program 4.1 List ten integers
#include <stdio.h>

int main(void)
{
    int count = 1;

    for (; count <= 10 ; ++count) {
        printf("  %d", count);
    }

    printf("\nAfter the loop count has the value %d.\n", count);
    return 0;
}
