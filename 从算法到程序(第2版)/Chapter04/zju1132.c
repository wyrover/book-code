#include <stdio.h>
#include <memory.h>

int C, T;
struct {
    int time, city;
} change[1000][100];
char name[100][100];
int start, dist;
int train[1000];
int travel[100];

int search(char *cityName)
{
    int i;

    for (i = 0; i < C; i++)
        if (!strcmp(name[i], cityName))
            return i;

    return -1;
}

void solve(int departure)
{
    int i, j, k;

    for (i = 0; i < C; i++)
        travel[i] = 1000000;

    travel[start] = departure;

    for (k = 0; k < C; k++)
        for (i = 0; i < T; i++)
            for (j = 0; j < train[i] - 1; j++)
                if (change[i][j].time >= travel[change[i][j].city]
                    && travel[change[i][j + 1].city] > change[i][j + 1].time)
                    travel[change[i][j + 1].city] = change[i][j + 1].time;

    for (i = 0; i < C; i++)
        if (i != dist)
            travel[i] = -1;

    for (k = 0; k < C; k++)
        for (i = 0; i < T; i++)
            for (j = 0; j < train[i] - 1; j++)
                if (change[i][j + 1].time <= travel[change[i][j + 1].city]
                    && travel[change[i][j].city] < change[i][j].time)
                    travel[change[i][j].city] = change[i][j].time;
}

int main()
{
    int i, j;
    char str[1000];
    int departure;
    int iCase = 0;

    while (scanf("%d", &C) && C) {
        for (i = 0; i < C; i++)
            scanf("%s", name[i]);

        scanf("%d", &T);

        for (i = 0; i < T; i++) {
            scanf("%d", &train[i]);

            for (j = 0; j < train[i]; j++) {
                scanf("%d%s", &change[i][j].time, str);
                change[i][j].city = search(str);
            }
        }

        scanf("%d", &departure);
        scanf("%s", str);
        start = search(str);
        scanf("%s", str);
        dist = search(str);
        solve(departure);
        printf("Scenario #%d\n", ++iCase);

        if (travel[dist] < 1000000) {
            printf("Departure %.4d %s\n", travel[start], name[start]);
            printf("Arrival   %.4d %s\n\n", travel[dist], name[dist]);
        } else
            printf("No connection\n\n");
    }

    return 0;
}
