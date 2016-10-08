#include <stdio.h>
#include <string.h>

#define MAXN 1000
#define MAXM 60
const int INF = 999999999;
char word[MAXN][MAXM];
int width;
int len[MAXN];
int line[MAXN];

void DP(int num)
{
    int i, j;
    int badness[MAXN];
    badness[num] = 0;

    for (i = num - 1; i >= 0; i--) {
        line[i] = 1;
        j = 1;
        int total = 0;
        int min = INF;

        while (i + j - 1 < num && total + len[i + j - 1] + j - 1 <= width) {
            total += len[i + j - 1];
            int score = badness[i + j];

            if (j == 1) score += 500;
            else {
                int q = (width - total) / (j - 1);
                int r = (width - total) % (j - 1);
                score += r * q * q + (j - 1 - r) * (q - 1) * (q - 1);
            }

            if (score <= min) {
                min = score;
                line[i] = j;
            }

            j++;
        }

        badness[i] = min;
    }
}

int main()
{
    int i, j;
    char ch[500];

    while (scanf("%d\n", &width) && width) {
        int word_num = 0;

        while (gets(ch) && strlen(ch)) {
            int pos = 0;

            while (ch[pos]) {
                sscanf(ch + pos, "%s", word[word_num]);
                len[word_num] = strlen(word[word_num]);
                pos += strlen(word[word_num++]);

                while (ch[pos] && ch[pos] == ' ') pos++;
            }
        }

        DP(word_num);
        int k = 0;

        while (k < word_num) {
            int total = 0;

            for (i = k; i < k + line[k]; i++)
                total += len[i];

            if (line[k] == 1) {
                printf("%s\n", word[k]);
                k ++;
                continue;
            }

            int q = (width - total) / (line[k] - 1);
            int r = (width - total) % (line[k] - 1);

            for (i = k; i < k + line[k] - 1; i++) {
                printf("%s", word[i]);
                int space = q + (i >= k + line[k] - 1 - r);

                for (j = 0; j < space; j++)
                    putchar(' ');
            }

            printf("%s\n", word[line[k] + k - 1]);
            k += line[k];
        }

        printf("\n");
    }

    return 0;
}
