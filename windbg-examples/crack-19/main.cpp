#include "windows.h"
#include "tchar.h"

int _tmain(int argc, TCHAR *argv[])
{
	int i = 1;

    do
    {
        printf("%d %s\n", i, "hello world!");
        i++;
    } 
    while (i < 10);

	return 0;
}