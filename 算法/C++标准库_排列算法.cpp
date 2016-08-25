#include <stdio.h>
#include <algorithm>

using namespace std;

#define n 3

int a[n] =
{
    1, 2, 3
};

int main()
{
    int i;

    while (1)
    {
        for (i = 0; i<n; i++)
            printf("%d ", a[i]);

        printf("\n");

        if (!next_permutation(a, a + n))
            break;
    }

    return 0;
}
