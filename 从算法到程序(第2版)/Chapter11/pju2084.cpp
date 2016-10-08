#include <stdio.h>

int ans[101][101];

void catalan()
{
    ans[0][0] = 1;
    ans[1][0] = 1;
    int i, j, k;
    int len = 1;

    for (i = 2; i <= 100; i++) {
        k = (4 * i - 2);
        int carry = 0;

        for (j = 0; j <= len; j++) {
            ans[i][j] = ans[i - 1][j] * k + carry;
            carry = ans[i][j] / 10;
            ans[i][j] %= 10;
        }

        while (carry) {
            ans[i][j++] = carry % 10;
            carry /= 10;
        }

        len = j;
        carry = 0;
        k = 0;

        while (j >= 0) {
            k = carry * 10 + ans[i][j];
            ans[i][j] = k / (i + 1);
            carry = k % (i + 1);
            j--;
        }
    }
}

int main()
{
    catalan();
    int n;

    while (scanf("%d", &n) && n > 0) {
        int i = 100;

        while (ans[n][i] == 0) i--;

        while (i >= 0) printf("%d", ans[n][i--]);

        printf("\n");
    }

    return 0;
}
