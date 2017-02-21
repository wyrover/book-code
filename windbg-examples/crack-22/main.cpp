#include "windows.h"
#include "tchar.h"

int a[3][4] = {
        { 1, 2, 3, 4 },
        { 5, 6, 7, 8 },
        { 9, 10, 11, 12 }
    };

int _tmain(int argc, TCHAR *argv[])
{
	
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 4; j++)
        {
            a[i][j] *= 2;
            printf("%d", a[i][j]);
        }
    }


	return 0;
}