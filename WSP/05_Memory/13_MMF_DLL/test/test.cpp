// test => D L L

#include <windows.h>

#pragma data_seg("AAA")
int x = 0;
int y = 0;
#pragma data_seg()

#pragma comment( linker, "/SECTION:AAA,RWS")

extern "C" __declspec( dllexport)
void SetData(int xx, int yy)
{
	x = xx;
	y = yy;
}

extern "C" __declspec( dllexport)
void GetData( int *xx, int *yy)
{
	*xx  = x;
	*yy  = y;
}
