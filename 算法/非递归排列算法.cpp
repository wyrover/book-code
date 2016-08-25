#include <stdio.h>

#define MAX 10

int N;

int solution[MAX];
int used[MAX];

void pailie()
{
    int i, j;
    int begin = 0;
    int step = 1;

    while (step != 1 || begin != N)
    {
        for (i = begin + 1; i<=N; i++)
        {
            if (!used[i])
            {
                used[i] = 1;
                solution[step++] = i;
                begin = 0;
                break;
            }
        }

        if (i == N + 1)
        {
            begin = solution[--step];
            used[begin] = 0;
        }

        if (step == N + 1)
        {
            for (j = 1; j<=N; j++)
                printf("%d ", solution[j]);

            printf("\n");

            begin = solution[--step];
            used[begin] = 0;
        }
    }
}

main()
{
    scanf("%d", & N);
    pailie();
}
