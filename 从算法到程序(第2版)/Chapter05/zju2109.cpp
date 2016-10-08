#include <iostream>
#include <algorithm>
#include <stdio.h>
using namespace std;

#define N  1005

struct trade {
    int java;
    int food;
    double ratio;
} a[N];

bool cmp(const trade& a, const trade& b)
{
    if (a.ratio > b.ratio) return true;

    return false;
}

int main()
{
    int m, n;

    while (scanf("%d%d", &m, &n) && (m + 1 || n + 1)) {
        int i;

        for (i = 0; i < n; i++) {
            scanf("%d%d", &a[i].java, &a[i].food);
            a[i].ratio = 1.0 * a[i].java / a[i].food;
        }

        sort(a, a + n, cmp);
        double beans = 0;
        int catfood = 0;

        for (i = 0; i < n && m - catfood >= a[i].food; i++) {
            beans += a[i].java;
            catfood += a[i].food;
        }

        if (i < n)
            beans += 1.0 * (m - catfood) / a[i].food * a[i].java;

        printf("%.3lf\n", beans);
    }

    return 0;
}
