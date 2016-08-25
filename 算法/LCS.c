#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int lcs( char *, char * );

int max( int, int );

int main( void )
{
    int large;
    char str1 [] = { "wyrover" };
    char str2 [] = { "wyhellorover" };

    large = lcs(str1, str2);
    printf("\nThe longest common string have %d letters.\n", large);

    system("pause");
    return 0;
}

int lcs( char * x, char * y )
{
    int lenx, leny;
    int data[50][50];
    char result[20];
    int re = 0;
    int dir_y = 10;

    int i, j;

    for (i = 0; i<50; i++)
        for (j = 0; j<50; j++)
            data[i][j] = 0;

    lenx = strlen(x);
    leny = strlen(y);

    //for(i = 0;i< lenx;i++)
    //           {
    //           for(j =0;j<leny;j++)
    //               printf("%d ",data[i][j]);
    //           puts("\n");
    //           }
    //puts("\n");
    //        puts("\t");
    //        for(j = 0;j <= leny;j++)
    //           printf("\t%c",*(y+j));
    //        puts("\n");
    for (i = 1; i<=lenx; i++)
    {
        //printf("%c  \t",*(x+i-1));
        for (j = 1; j<=leny; j++)
        {
            if (*(x + i - 1) == *(y + j - 1))
                data[i][j] = data[i - 1][j - 1] + 1;
            else
                data[i][j] = max(data[i][j - 1], data[i - 1][j]);
        //printf("%d\t",data[i][j]);
        }
    //puts("\n");
    }

    //for(i = 0;i <= lenx;i++)
    //           {
    //           for(j = 0;j <= leny;j++)
    //               printf("%d ",data[i][j]);
    //           puts("\n");
    //           }

    for (i = lenx; i>0; i--)
        for (j = leny; j>0; j--)
        {
            if (data[i][j]>data[i][j - 1] && j<dir_y)
            {
                result[re++] = *(x + i - 1);
                //printf("%d\t%d\t%c\n",i,j,*(x+i-1));
                dir_y = j;
                break;
            }
        }

    for (i = re - 1; i>=0; i--)
        printf("%c\t", result[i]);

    return (data[lenx][leny]);
}

int max( int a, int b )
{
    int c;

    if (a>=b)
        c = a;
    else
        c = b;

    return c;
}
