#include <windows.h>
#include <iostream>
using namespace std;

typedef void(*F)(DWORD tid);

void main()
{
	HMODULE hDll = LoadLibrary( "HK2.dll" );

	F Install = (F)GetProcAddress(hDll, "Install");

	if(Install == 0)
		cout << "Error" << endl;
	else
		Install(0);

	int n;
	cin >> n;

	// 자원 해지.. 생략...!! - 프로세스가 죽으면 자동으로 해지는 된다. 
	// 하지만 해지는 꼭 하는 습관을 들이도록...
}

