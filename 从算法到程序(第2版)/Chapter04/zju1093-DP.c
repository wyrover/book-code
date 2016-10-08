#include <stdio.h>
#define NUM 100
#define MAX 9999999

int box[NUM][3];
int height[NUM];
int number;

void oriente(int serial, int a, int b, int c)
{
    box[serial][0] = a;
    box[serial][1] = b;
    box[serial][2] = c;
}

int DP(int serial)
{
    if (height[serial] != -1) return height[serial];

    if (serial > number) return 0;

    int t = 0;
    int i;

    for (i = 1; i <= number; ++i) {
        if ((box[i][0] < box[serial][0] && box[i][1] < box[serial][1]) ||
            (box[i][1] < box[serial][0] && box[i][0] < box[serial][1]))
            t = DP(i) + box[i][2];

        if (t > height[serial]) height[serial] = t;
    }

    return height[serial];
}

int main()
{
    int n;
    int cases = 0;
    int a, b, c;

    while (scanf("%d", &n) && n) {
        box[0][0] = box[0][1] = box[0][2] = MAX;
        number = 0;
        int i;

        for (i = 0; i < n; ++i) {
            scanf("%d%d%d", &a, &b, &c);
            oriente(++number, a, b, c);
            oriente(++number, b, c, a);
            oriente(++number, c, a, b);
        }

        for (i = 0; i <= number; ++i)
            height[i] = -1;

        printf("Case %d: maximum height = %d\n", ++cases, DP(0));
    }

    return 0;
}
