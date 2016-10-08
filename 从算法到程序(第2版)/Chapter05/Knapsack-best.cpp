#include<iostream>
#include<algorithm>
using namespace std;

struct bag {
    int w;
    int v;
    double x;
    int index;
    double c;
} a[1001];

bool cmp(bag a, bag b)
{
    if (a.c > b.c) return true;

    return false;
}

double knapsack(int n, bag a[], double c)
{
    double cleft = c;
    int i = 0;
    double b = 0;

    while (i < n && a[i].w <= cleft) {
        cleft -= a[i].w;
        b += a[i].v;
        a[a[i].index].x = 1.0;
        i++;
    }

    if (i < n) {
        a[a[i].index].x = 1.0 * cleft / a[i].w;
        b += a[a[i].index].x * a[i].v;
    }

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
            a[i].x = 0;
            a[i].index = i;
        }

        sort(a, a + n, cmp);
        knapsack(n, a, c);
        printf("%15.3lf\n", knapsack(n, a, c));

        for (i = 0; i < n; i++)
            if (a[i].x == 0) printf("0 ");
            else if (a[i].x == 1) printf("1 ");
            else printf("%.3lf ", a[i].x);

        printf("\n");
    }

    return 0;
}
