// dll dir 얻어오기 

/*

*/

#include <windows.h>
#include <iostream>
using namespace std;

void main()
{
	HMODULE	h1 = GetModuleHandle("kernel32.dll");
	char buff[1024];
	GetModuleFileName(h1, buff, 1024);
	cout << buff << endl;
}