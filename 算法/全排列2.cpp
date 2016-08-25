#include <stdio.h>

#define MAX 10

int used[MAX];

int result[MAX];
int N;

void print()
{
    int i;

    for (i = 0; i < N; i++)
        printf("%d ", result[i]);

    printf("\n");
}

void proc(int step)
{
    int i;

    if (step == N)
        print();
    else {
        for (i = 0; i < N; i++) {
            if (!used[i]) {
                used[i] = 1;
                result[step] = i + 1;
                proc(step + 1);
                used[i] = 0;
            }
        }
    }
}

main()
{
    scanf("%d", & N);
    proc(0);
}
