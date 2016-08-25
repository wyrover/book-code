// MyKMP.cpp : Defines the entry point for the console application. 

#include "stdio.h"

int next[255];

char pat [] = "aacaab";
char str [] = "fsoifhewgsdksdkfhsdifhewhfefksdhfdsoihfoehfosdhfdsaacaab";

void getNext( char pat [], int next [] )
{
    int j = 0;

    for (int i = 0; pat[i]; i++)
    {
        if (i == 0)
            next[i] = -1;
        else
        {
            if (pat[i] == pat[j])
            {
                ++i;
                ++j;
                next[i] = j;
            }
            else
                j = next[j];
        }
    }
}

int KMP( char *str1, char *pat, int *next )
{
    int i = 0, j = 0;

    while (str[i])
    {
        if (pat[j] == 0)
            return i - j;

        if (j == 0 || str[i] == pat[j])
        {
            ++i;
            ++j;
        }
        else
            j = next[j];
    }

    if (pat[j] == 0)
        return i - j;

    return -1;
}

int main( int argc, char *argv [] )
{
    int i;
    getNext(pat, next);
    int result = KMP(str, pat, next);
    printf("%s\n", str);

    for (i = 0; i<result; i++)
        printf(" ");

    printf("%s\n", pat);
    printf("at %d\n", result);
    return 0;
}
