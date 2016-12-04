
#include <windows.h>
#include <stdio.h>

typedef int (*AddPtr)(int, int, int, int);

void main()
{
	HMODULE h = LoadLibraryA("Delphidll.dll");
	if (NULL == h)
	{
		printf("cannot load\n");
		return;
	}
	AddPtr fp = (AddPtr)GetProcAddress(h, "Add");
	if (NULL == fp)
	{
		printf("cannot get function sub\n");
		return;
	}	
	int i = fp(1, 2, 3, 4);
	printf("sum = %d\n", i);
	getchar();
}