#include <stdio.h>
#include <windows.h>

typedef int (*SubPtr)(int, int);

void main()
{
	//由于dll_relocation.dll和testdll.exe被自动生成到一个目录，所以dll名字没有路径。
	HMODULE h = LoadLibraryA("dll_relocation.dll");
	if (NULL == h)
	{
		printf("cannot load\n");
		return;
	}
	SubPtr fp = (SubPtr)GetProcAddress(h, "sub");
	if (NULL == fp)
	{
		printf("cannot get function sub\n");
		return;
	}	
	printf("3 - 2 = %d\n", fp(3, 2));
}