#include <stdio.h>
#include <algorithm>
#include <iostream>
using namespace std;

struct mouse {
    int weight, speed, id;
} mice[1001];

bool cmp(const mouse a, const mouse b)
{
    if (a.weight == b.weight) {
        if (b.speed > a.speed) return true;
        else return false;
    } else return a.weight < b.weight;
}

void output(int path[], int pos)
{
    if (pos == 0) return;

    output(path, path[pos]);
    printf("%d\n", mice[pos].id);
}

int main()
{
    int n = 1;

    while (scanf("%d%d", &mice[n].weight, &mice[n].speed) != EOF) {
        mice[n].id = n;
        n++;
    }

    sort(mice, mice + n, cmp);
    int count[1001] = {0};
    int path[1001] = {0};
    count[1] = 1;

    for (int i = 2; i < n; i++) {
        for (int j = 1; j < i; j++)
            if (mice[i].weight > mice[j].weight && mice[i].speed < mice[j].speed)
                if (count[i] < count[j]) {
                    count[i] = count[j];
                    path[i] = j;
                }

        count[i]++;
    }

    int max = 0;
    int pos;

    for (int i = 1; i < n; i++)
        if (count[i] > max) {
            max = count[i];
            pos = i;
        }

    printf("%d\n", max);
    output(path, pos);
    return 0;
}
