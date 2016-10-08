#include<stdio.h>
#include<iostream>
#include<algorithm>
using namespace std;

int m;
int num;
int a[35];
int pos[31];
bool exist;

void print(int sum)
{
    for (int i = pos[num - 1] + 1; i < m; i++)
        if (sum == a[i]) {
            exist = true;
            printf("%d", a[pos[0]]);

            for (int i = 1; i < num; i++)
                printf("+%d", a[pos[i]]);

            printf("=%d\n", sum);
            return;
        }
}

int Search(int depth, int sum, int i)
{
    if (depth == num) print(sum);
    else
        for (; i < m - 1; i++) {
            pos[depth] = i;

            if (sum + a[i] > a[m - 1])  return 0;

            if (!Search(depth + 1, sum + a[i], i + 1) && pos[depth + 1] - i == 1)
                return 0;
        }

    return 1;
}

int main()
{
    int iCase;
    scanf("%d", &iCase);

    while (iCase--) {
        scanf("%d", &m);

        for (int i = 0; i < m; i++)
            scanf("%d", &a[i]);

        sort(a, a + m);
        exist = false;

        for (num = 2; num < m; num++)
            Search(0, 0, 0);

        if (!exist)  printf("Can't find any equations.\n");

        printf("\n");
    }

    return 0;
}
