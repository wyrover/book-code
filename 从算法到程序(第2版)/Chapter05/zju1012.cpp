#include<stdio.h>
#include<string.h>
#include<algorithm>
#include<iostream>
using namespace std;

#define Max 10001
struct Node {
    int a, b;
    int t, u;
    int v, w, x;
};

Node Job[Max];

bool cmp(Node a , Node b)
{
    if (a.t != b.t) return a.t < b.t;
    else return a.v > b.v;
}

int main()
{
    bool Finished[Max];
    int i, j;
    int iCase = 0;
    int F;

    while (scanf("%d", &F) && F) {
        iCase++;
        int Profit = 0;
        memset(Finished, 0, sizeof(Finished));
        int M, N, L;
        scanf("%d%d%d", &M, &N, &L);

        for (i = 0; i < L; i++)
            scanf("%d%d%d%d%d%d%d", &Job[i].a,
                  &Job[i].b,  &Job[i].t, &Job[i].u,
                  &Job[i].v, &Job[i].w, &Job[i].x);

        sort(Job, Job + L, cmp);
        int count = 0;

        for (i = 0; i < F && count < L; i++) {
            int cpu = M;
            int memory = N;

            for (j = 0; j < L; j++) {
                if (Job[j].t > i)  break;

                if (!Finished[j] && cpu >= Job[j].a && memory >= Job[j].b) {
                    cpu -= Job[j].a;
                    memory -= Job[j].b;
                    Finished[j] = 1;
                    Profit += Job[j].v;

                    if (i + 1 <= Job[j].u)
                        Profit += (Job[j].u - i - 1) * Job[j].w;
                    else
                        Profit -= (i + 1 - Job[j].u) * Job[j].x;

                    count++;
                }
            }
        }

        for (i = 0; i < L; i++)
            if (!Finished[i] && Job[i].u <= F)
                Profit -= (F - Job[i].u) * Job[i].x;

        printf("Case %d: %d\n\n", iCase, Profit);
    }

    return 0;
}
