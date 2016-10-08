#include <iostream>
using namespace std;

#define NUM 100
int n;
int m;
int a[NUM][NUM];
int x[NUM];
int sum ;

bool Same(int t)
{
    int i;

    for (i = 1; i <= n; i++)
        if ((a[t][i] == 1) && (x[i] == x[t]))
            return false;

    return true;
}

void BackTrack(int t)
{
    int i;

    if (t > n) {
        sum ++ ;

        for (i = 1; i <= n ; i++)
            printf("%d ", x[i]);

        printf("\n") ;
    } else
        for (i = 1; i <= m; i++) {
            x[t] = i;

            if (Same(t))    BackTrack(t + 1);

            x[t] = 0;
        }
}

int main()
{
    scanf("%d %d", &n, &m);
    memset(a, 0, sizeof(a));
    int b , e ;

    while (scanf("%d%d", &b, &e) && (b || e)) {
        a[b][e] = 1 ;
        a[e][b] = 1 ;
    }

    sum = 0;
    BackTrack(1);
    printf("Total=%d\n", sum) ;
    return 0;
}
