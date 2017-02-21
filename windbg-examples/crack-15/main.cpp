#include "windows.h"
#include "tchar.h"

int _tmain(int argc, TCHAR *argv[])
{
	int i = 1;

    switch (i)
    {
    case 1: printf("%s", "no1");
    case 2: printf("%s", "no2");
    default: printf("%s", "default");    
    }

	return 0;
}