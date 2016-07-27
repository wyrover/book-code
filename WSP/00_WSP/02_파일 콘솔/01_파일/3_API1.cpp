//---------------------------------------------------
//  동시 접근 제어 B
//---------------------------------------------------

#include <windows.h>
#include <stdio.h>


// 에러 메시지 출력
void PrintErrorMsg( DWORD err)
{
	LPTSTR	lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, err, 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("%s ", lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int main()
{
	
	HANDLE hf = CreateFile("c://test.dat", GENERIC_READ,
		FILE_SHARE_READ,  // 에러 발생 
//		FILE_SHARE_WRITE, // 성공 
		NULL, OPEN_ALWAYS, 0, NULL);

	if( hf != INVALID_HANDLE_VALUE)
	{
		char	buf[256];
		DWORD	read;
		printf("열기 성공\n\n");
		ReadFile(hf, buf, sizeof(buf), &read, NULL);
		buf[read] = '\0';
		printf("%s", buf);
		CloseHandle(hf);
	}
	else
	{
		PrintErrorMsg(GetLastError());
	}
	getchar();

	return 0;
}