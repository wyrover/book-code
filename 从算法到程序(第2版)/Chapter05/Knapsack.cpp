#include<iostream>
#include<algorithm>
using namespace std;

struct bag {
    int w;
    int v;
    double c;
} a[1001];

bool cmp(bag a, bag b)
{
    return a.c >= b.c;
}

double knapsack(int n, bag a[], double c)
{
    double cleft = c;
    int i = 0;
    double b = 0;

    while (i < n && a[i].w < cleft) {
        cleft -= a[i].w;
        b += a[i].v;
        i++;
    }

    if (i < n) b += 1.0 * a[i].v * cleft / a[i].w;

    return b;
}

int main()
{
    int c;
    int n;
    int i;

    while (scanf("%d %d", &c, &n) && c) {
        for (i = 0; i < n; i++) {
            scanf("%d%d", &a[i].w, &a[i].v);
            a[i].c = 1.0 * a[i].v / a[i].w;
        }

        sort(a, a + n, cmp);
        printf("%15.3lf\n\n", knapsack(n, a, c));
    }

    return 0;
}
