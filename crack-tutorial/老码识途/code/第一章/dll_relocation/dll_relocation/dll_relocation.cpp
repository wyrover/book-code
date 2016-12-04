// 这是主 DLL 文件。

#include "dll_relocation.h"


int gi;

int sub(int x, int y)
{
	gi = 0x12345678;
	return x - y;
}