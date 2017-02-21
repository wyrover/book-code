#include "windows.h"
#include "tchar.h"

int _tmain(int argc, TCHAR *argv[])
{
	int i = 148;

    switch (i)
    {
    case 1001: printf("%s", "no1");
    case 22: printf("%s", "no2");
    case 388: printf("%s", "no3");
    case 777: printf("%s", "no4");
    case 48: printf("%s", "no5");
    case 998: printf("%s", "no6");
    case 148: printf("%s", "no7");
    case 592: printf("%s", "no8");
    default: printf("%s", "default");    
    }

	return 0;
}