#include <iostream>
#include <algorithm>
using namespace std;

struct load {
    int index;
    int w;
} box[1001];

bool cmp(load a, load b)
{
    if (a.w < b.w) return true;
    else return false;
}

int main()
{
    int c, n;
    int x[1001];

    while (scanf("%d%d", &c, &n) != EOF) {
        memset(box, 0, sizeof(box));
        memset(x, 0, sizeof(x));

        for (int i = 1; i <= n; i++) {
            scanf("%d", &box[i].w);
            box[i].index = i;
        }

        stable_sort(box, box + n + 1, cmp);

        if (box[1].w > c) {
            printf("No answer!\n");
            continue;
        }

        int i;

        for (i = 1; i <= n && box[i].w <= c; i++) {
            x[box[i].index] = 1;
            c -= box[i].w;
        }

        printf("%d\n", i - 1);

        for (i = 1; i <= n; i++)
            if (x[i]) printf("%d ", i);

        printf("\n");
    }

    return 0;
}
