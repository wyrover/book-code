#include <iostream>
#include <cmath>
using namespace std;

#define NUM 20
int n;
int x[NUM];
int sum;

inline bool Place(int t)
{
    int i;

    for (i = 1; i < t; i++)
        if ((abs(t - i) == abs(x[i] - x[t])) || (x[i] == x[t]))
            return false;

    return true;
}

void Backtrack(int t)
{
    int i;

    if (t > n) {
        sum++;

        for (i = 1; i <= n; i++)
            printf(" %d", x[i]);

        printf("\n");
    } else
        for (i = 1; i <= n; i++) {
            x[t] = i;

            if (Place(t)) Backtrack(t + 1);
        }
}

int main()
{
    while (cin >> n) {
        sum = 0;
        Backtrack(1);
        printf("Total= %d\n\n", sum);
    }

    return 0;
}
