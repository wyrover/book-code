//---------------------------------------------------------
// 보안 설명자 - 설정 
//---------------------------------------------------------

#pragma comment( linker, "/subsystem:windows")

#include <windows.h>
#include <aclapi.h>

void SetSecurity()
{
	HANDLE hFile;
	char str[] = "누구나 읽고 쓸 수 있으며 User1만 쓸 수 있다.";
	DWORD dwWritten;

	// ACE 관련 구조체 필요한 수만큼 정의 : 
	// 누구에게 어떠한 권한을 줄 것인가??
	EXPLICIT_ACCESS EA[2];
	char pSid[255];
	char pDomain[255];
	DWORD cbSid=255, cbDomain = 255;
	SID_NAME_USE peUse;
	PACL	pAcl;
	SECURITY_DESCRIPTOR	SD;
	SECURITY_ATTRIBUTES SA;

	// 1. 모든 SID를 구한다.
	SID_IDENTIFIER_AUTHORITY SIDEvery = SECURITY_WORLD_SID_AUTHORITY;
	PSID pEverySID;
	// 기정 SID(Every, Administrator)를 구하는 함수 : 
	// 모든 사용자를 지칭하는 SID를 pEverySID로 저장 
	AllocateAndInitializeSid(&SIDEvery, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0,
		&pEverySID);
	
	memset(EA, 0, sizeof(EXPLICIT_ACCESS)*2);
	// Every에 읽기 권한(GENERIC_READ)을 허가(Set_ACCESS) ACE 생성 
	EA[0].grfAccessPermissions = GENERIC_READ;
	EA[0].grfAccessMode		   = SET_ACCESS;
	EA[0].grfInheritance	   = NO_INHERITANCE;
	EA[0].Trustee.TrusteeForm	= TRUSTEE_IS_SID;
	EA[0].Trustee.TrusteeType	= TRUSTEE_IS_WELL_KNOWN_GROUP;
	EA[0].Trustee.ptstrName     = (LPTSTR)pEverySID;

	// 1.1 User1의 SID를 구한다.
	// 사용자의 이름으로 부터 특정 SID를 구하는 함수 
	// pSid에 저장된다.
	LookupAccountName(NULL, "User1", (PSID)pSid, &cbSid, pDomain, &cbDomain, &peUse);
	
	// ACE를 만든다.
	// 누구에게 : SID를 대입하면 됨
	// 어떠한 권한을 줄 것인가   : 각각에 대응되는 맴버에 설정 
	// User1에게 모든 권한을 허가...
	EA[1].grfAccessPermissions	= GENERIC_ALL;
	EA[1].grfAccessMode			= SET_ACCESS;
	EA[1].grfInheritance		= NO_INHERITANCE;
	EA[1].Trustee.TrusteeForm	= TRUSTEE_IS_SID;
	EA[1].Trustee.TrusteeType	= TRUSTEE_IS_USER;
	EA[1].Trustee.ptstrName     = (LPTSTR)pSid;

	// ACE(조각)를 ACL에 포함시키고 새 ACL(List)을 만든다.
	SetEntriesInAcl(2, EA, NULL, &pAcl);

	// SD를 초기화 한다.
	InitializeSecurityDescriptor(&SD, SECURITY_DESCRIPTOR_REVISION);

	// ACL을 SD(보안 해설자)에 포함시킨다.
	SetSecurityDescriptorDacl(&SD, TRUE, pAcl, FALSE);

	// 보안 속성을 만든다.
	SA.nLength		= sizeof(SECURITY_ATTRIBUTES);
	SA.lpSecurityDescriptor	= &SD;
	SA.bInheritHandle		= FALSE;

	// 파일을 생성한다.
	hFile = CreateFile("c:\\User1Only.txt", GENERIC_ALL, 0, &SA,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, str, strlen(str), &dwWritten, NULL);
	CloseHandle(hFile);
	
	//실행중에 할당한 메모리 해제
	FreeSid(pEverySID);
	LocalFree(pAcl);
	// 폴더상에 제대로 만들어 졌는지 확인...

}
HINSTANCE g_hInst;
// 메세지 처리함수.
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch( msg )
	{
	case WM_DESTROY: 
		PostQuitMessage(0); 
		return 0;
	}
	return DefWindowProc( hwnd, msg, wParam, lParam);
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, PSTR lpCmdLine, int nShowCmd)
{
	WNDCLASS wc;

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH);

	wc.hCursor  = LoadCursor( 0, IDC_ARROW );
	wc.hIcon    = LoadIcon  ( 0, IDI_WINLOGO);
	wc.hInstance= hInstance; 

	wc.lpfnWndProc   = WndProc;
	wc.lpszClassName = "First"; 
	wc.lpszMenuName  = 0; 
	wc.style		 = 0; 

	ATOM atom = RegisterClass( &wc );

	HWND hwnd = CreateWindowEx( 0, "First", "Spy", 
					WS_OVERLAPPEDWINDOW, 0, 0, 500,500, 0, 0, hInstance, 0);

	ShowWindow( hwnd, SW_SHOW);
	g_hInst = hInstance;

	SetSecurity();
	MessageBox(NULL,"보안 설명자를 생성했습니다", "알림",MB_OK);

	MSG msg;
	while ( GetMessage( &msg, 0, 0, 0) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return msg.wParam;
}



