#include <stdio.h>
#include <windows.h>

typedef int (*SubPtr)(int, int);

void main()
{
	
	HMODULE h_withoutRel = LoadLibraryA("dll_relocation.dll");
	if (NULL == h_withoutRel)
	{
		printf("cannot load\n");
		return;
	}
	SubPtr fpWithoutRel = (SubPtr)GetProcAddress(h_withoutRel, "sub");
	if (NULL == fpWithoutRel)
	{
		printf("cannot get function sub without relocation\n");
		return;
	}
	HMODULE h_withRel = LoadLibraryA("dll2.dll");
	if (NULL == h_withRel)
	{
		printf("cannot load\n");
		return;
	}
	SubPtr fpWithRel = (SubPtr)GetProcAddress(h_withRel, "sub");
	if (NULL == fpWithRel)
	{
		printf("cannot get function sub with relocation\n");
		return;
	}
	fpWithoutRel(3, 2);
	fpWithRel(3, 2);
}