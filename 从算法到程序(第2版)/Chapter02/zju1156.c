#include <stdio.h>
#include <memory.h>

int test[256];
int secret[400];

int main()
{
    int i, j, k;
    int n;
    int m;
    int intensity;
    int N;          //数据块的数量
    scanf("%d", &N);

    while (N--) {
        int num;
        int iCase;  //测试例的数量
        scanf("%d", &iCase);

        for (num = 1; num <= iCase; num++) {
            scanf("%d%d", &n, &m);

            for (i = 0; i < m; i++) {
                scanf("%d %d", &k, &intensity);
                test[intensity] = k;
            }

            scanf("%d", &m);
            memset(secret, 0xff, sizeof(secret));

            for (i = 0; i < m; i++) {
                scanf("%d %d", &k, &intensity);
                secret[k] = intensity;
            }

            if (num > 1) printf("\n");

            printf("Case %d\n\n", num);

            for (i = 0; i < n; i++) {
                for (j = 0; j < n; j++) {
                    k = test[i * n + j];

                    while (secret[k] == -1)
                        k = (k - 1) / 4;

                    printf("%4d", secret[k]);
                }

                printf("\n");
            }
        }

        if (N) printf("\n");
    }

    return 0;
}
