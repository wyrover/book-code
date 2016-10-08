#include<stdio.h>
#include<iostream>
#include<algorithm>
using namespace std;

#define N 51
char image[N][N];
int sum;
int w, h;

void Dots(int i, int j)
{
    if (i < 0 || i >= h || j < 0 || j >= w || image[i][j] != 'X') return ;

    image[i][j] = '*';
    Dots(i - 1, j);
    Dots(i + 1, j);
    Dots(i, j - 1);
    Dots(i, j + 1);
}

void Dice(int i, int j)
{
    if (i < 0 || i >= h || j < 0 || j >= w || image[i][j] == '.') return ;

    if (image[i][j] == 'X') {
        sum++;
        Dots(i, j);
    }

    image[i][j] =  '.';
    Dice(i - 1, j);
    Dice(i + 1, j);
    Dice(i, j - 1);
    Dice(i, j + 1);
}

int main()
{
    int i, j;
    int num[N * N];
    int number = 1;
    int count;

    while (scanf("%d%d", &w, &h) && (w || h)) {
        count = 0;

        for (i = 0; i < h; i++)
            scanf("%s", image[i]);

        for (i = 0; i < h; i++)
            for (j = 0; j < w; j++) {
                if (image[i][j] == '.') continue;

                sum = 0;

                if (image[i][j] == '*') Dice(i, j);
                else if (image[i][j] == 'X') {
                    sum++;
                    Dots(i, j);
                    Dice(i, j);
                }

                num[count++] = sum;
            }

        sort(num, num + count);
        printf("Throw %d\n", number++);

        for (i = 0; i < count; i++) {
            if (i) printf(" ");

            printf("%d", num[i]);
        }

        printf("\n\n");
    }

    return 0;
}
