#include "windows.h"
#include "tchar.h"

int _tmain(int argc, TCHAR *argv[])
{
	int i = 1;

    while (i < 10) {
        printf("%d %s\n", i, "hello world!");
        i++;
    }

	return 0;
}