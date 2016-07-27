#include <windows.h>
#include <conio.h>
#include <stdio.h>
// DLL의 명시적 연결은 헤더와 라이브러리 파일이 필요 없다.
// DLL 자체만 있으면 사용 가능 하다. - 단 함수의 signature를 미리 알고 있어야 한다.

typedef int(*FUNC)(int, int);

void main()
{
	getch();
	HMODULE hDll = LoadLibrary("XYZ.dll");
	if ( hDll == 0 )
	{
		printf("xyz.dll 을 찾을수가 없습니다.\n");
		return;
	}
	printf("DLL이 Load된 주소 : %p\n", hDll );
	//----------------------------------------------
	FUNC f = (FUNC)GetProcAddress( hDll, "Add");c

	if ( f == 0 )
		printf("DLL 안에서 해당 함수를 찾을수가 없습니다.\n");
	else
	{
		printf("Add함수 주소 : %p\n", f);
		printf("10 + 20 = %d\n", f(10,20));
	}
	FreeLibrary( hDll ); // DLL 해지
}

// 반드시 알아야 하는 함수 3개 
// LoadLibrary()  / FreeLibrary()    / GetProcAddress()



















