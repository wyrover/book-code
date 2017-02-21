#include "windows.h"
#include "tchar.h"

int _tmain(int argc, TCHAR *argv[])
{
	int a[5] = { 1, 2, 3, 4, 5 };
    int i;
    for (i = 0; i < 5; i++)
    {
        a[i]+=1;
    }

	return 0;
}