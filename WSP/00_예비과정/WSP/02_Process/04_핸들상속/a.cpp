#include <windows.h>

void main()
{
	// 상속 가능한 KO 만들기
	SECURITY_ATTRIBUTES sa;

	sa.nLength	= sizeof( sa );
	sa.bInheritHandle = TRUE;    // 상속가능하게
	sa.lpSecurityDescriptor = 0; // 실제 보안정보.

	HANDLE hEvent = CreateEvent(  &sa, // 보안
								 0, 0, "e");

	// 상속가능하게 바꾸기..
	SetHandleInformation( hEvent, HANDLE_FLAG_INHERIT,
							      HANDLE_FLAG_INHERIT);


	CloseHandle( hEvent );
}