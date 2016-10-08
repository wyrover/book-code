#include <iostream>
using namespace std;

char cMap[5][5];
int iBest, n;

bool CanPut(int row,  int col)
{
    int i;

    for (i = row - 1; i >= 0; i--) {
        if (cMap[i][col] == 'O') return false;

        if (cMap[i][col] == 'X') break;
    }

    for (i = col - 1; i >= 0; i--) {
        if (cMap[row][i] == 'O') return false;

        if (cMap[row][i] == 'X') break;
    }

    return true;
}

void solve(int k, int current)
{
    int x, y;

    if (k == n * n) {
        if (current > iBest) {
            iBest = current;
            return;
        }
    } else {
        x = k / n;
        y = k % n;

        if (cMap[x][y] == '.' && CanPut(x, y)) {
            cMap[x][y] = 'O';
            solve(k + 1, current + 1);
            cMap[x][y] = '.';
        }

        solve(k + 1, current);
    }
}

int main()
{
    int i, j;

    while (scanf("%d", &n) && n) {
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                cin >> cMap[i][j];

        iBest = 0;
        solve(0, 0);
        printf("%d\n", iBest);
    }

    return 0;
}
