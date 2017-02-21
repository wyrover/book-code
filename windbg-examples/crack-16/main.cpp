#include "windows.h"
#include "tchar.h"

int _tmain(int argc, TCHAR *argv[])
{
	int i = 1;

    switch (i)
    {
    case 1: printf("%s", "no1");
    case 2: printf("%s", "no2");
    case 3: printf("%s", "no3");
    case 4: printf("%s", "no4");
    case 5: printf("%s", "no5");
    case 6: printf("%s", "no6");
    default: printf("%s", "default");    
    }

	return 0;
}