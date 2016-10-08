#include<iostream>
using namespace std;

#define NUM 100
int c;
int n;
int cw;
int cv;
int bestv;

struct Object {
    int w;
    int v;
    double d;
} Q[NUM];

bool cmp(Object a, Object b)
{
    if (a.d >= b.d) return true;
    else return false;
}

int Bound(int i)
{
    int cleft = c - cw;
    int b = cv;

    while (i < n && Q[i].w <= cleft) {
        cleft -= Q[i].w;
        b += Q[i].v;
        i++;
    }

    if (i < n) b += cleft * Q[i].d;

    return b;
}

void backtrack(int i)
{
    if (i + 1 > n) {
        bestv = cv;
        return;
    }

    if (cw + Q[i].w <= c) {
        cw += Q[i].w;
        cv += Q[i].v;
        backtrack(i + 1);
        cw -= Q[i].w;
        cv -= Q[i].v;
    }

    if (Bound(i + 1) > bestv) backtrack(i + 1);
}

int main()
{
    while (scanf("%d", &c) && c) {
        cw = 0;
        cv = 0;
        bestv = 0;
        scanf("%d", &n);

        for (int i = 0; i < n; i++) {
            scanf("%d%d", &Q[i].w, &Q[i].v);
            Q[i].d = 1.0 * Q[i].v / Q[i].w;
        }

        sort(Q, Q + n, cmp);
        backtrack(0);
        printf("%d\n", bestv);
    }

    return 0;
}
