#include "windows.h"
#include "tchar.h"

int _tmain(int argc, TCHAR *argv[])
{
	int i;
    for (i = 1; i <10; i++)
    {
        printf("%d %s\n", i, "hello world!");
    }

	return 0;
}