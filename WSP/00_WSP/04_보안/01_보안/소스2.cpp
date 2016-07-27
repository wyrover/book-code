//------------------------------------------------------------------
// 파일의 보안 정보를 조사
//------------------------------------------------------------------
#pragma comment( linker, "/subsystem:windows")


#include <windows.h>
#include "AclApi.h"

//----------------------------------------------------------------------------
// GetSecurity
// 파일 정보를 조사해 보여주는 예제 
// 
//----------------------------------------------------------------------------
char gMes[1024] = "마우스 왼쪽 버튼  클릭시 보안 정보를 보고자하는 파일 선택";
void GetSecurity()
{
	OPENFILENAME OFN;
	char lpstrFile[MAX_PATH] = "";
	HANDLE hFile;
	char Mes[255];
	PSECURITY_DESCRIPTOR pSD;
	PSID pOwner;
	PACL pDacl;
	int i;
	DWORD cbName, cbDomain;
	char *Name, *Domain;
	SID_NAME_USE	peUse;
	ULONG	nAce;
	PEXPLICIT_ACCESS pEntry;

	// 1. 대상 파일의 이름을 얻는다.
	memset(&OFN, 0, sizeof(OPENFILENAME));

	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner	= NULL;
	OFN.lpstrFilter	= "Every File(*.*)\0*.*\0";
	OFN.lpstrFile	= lpstrFile;
	OFN.nMaxFile	= 256;
	OFN.lpstrInitialDir	= "c:\\";
	if(GetOpenFileName(&OFN)==0){
		return;
	}
	//------------------------------------------------------------



	// 2. 파일을 연다.
	hFile = CreateFile(lpstrFile, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == NULL)
		return;

	// 파일명 출력
	wsprintf(Mes, "파일명 = %s\n", lpstrFile);
	strcpy(gMes, Mes);


	
	//3. 보안 설명자를 읽는다.---------------------------------------------------

	// 파일 핸들로 부터 보안 설명자를 구하는 함수 
	// 소유자 정보, DACL 정보를 구한다. 
	//1) 대상 오브젝트의 핸들
	//2) 보안 오브젝트의 종류를 지정한 열거형 값  
	//   파일일 경우는 SE_FILE_OBJECT 값으로 지정
	//3) 보안 설명자의 정보중 구하고자 하는 정보의 종류 
	//   - 소유자 정보|DACL 정보 
	//4) 나머지 4개의 인수는 정보를 돌려받기 위한 포인터 변수들
	// 마지막은 보안 설명자 그 자체를 돌려받기 위한 보안 설명자 구조체 포인터 
	//   - pOwner : 오브젝트를 소유한 소유자에 대한 정보 획득(SID형태임)
	//   - pDacl  : DACL의 정보 획득 
	if(GetSecurityInfo(hFile, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION|
		DACL_SECURITY_INFORMATION, &pOwner, NULL, &pDacl, NULL, (LPVOID*)&pSD)!=
		ERROR_SUCCESS){
		MessageBox(NULL,"보안 설명자 읽을 수 없다.", "", MB_OK);
		return;
	}
	CloseHandle(hFile);

	// 소유자 정보 보여주기
	cbName = 0;
	cbDomain = 0;
	// pOwner로 들어온 소유자 접근 포인터를 가지고 정보 확인 



	// 4. 소유자 정보 보기 
	//    먼저 name과 domain에 버퍼 길이를 조사한다. -----------------------
	//    LookupAccountSid()  : sid ==> 사용자의 이름 얻기 
	LookupAccountSid(NULL, pOwner, NULL, &cbName, NULL, &cbDomain, &peUse);
	Name = (char*)malloc(cbName);
	Domain = (char*)malloc(cbDomain);

	// 먼저 SID로 부터 사용자의 이름을 구한다. 
	// 소유자의 정보는 GetSecurtyInfo 함수를 통해 pOwner에 들어 있다.
	// pOwner의 정보는 SID 형이기 때문에 실제 누구를 가리키는지 알수 없다.
	// 따라서 SID로부터 계정 정보를 구해야 한다.
	LookupAccountSid(NULL, pOwner, Name, &cbName, Domain, &cbDomain, &peUse);

	wsprintf(Mes, "소유자 = %s\n", Name);
	strcat(gMes, Mes);

	wsprintf(Mes, "소유자 도메인 = %s\n", Domain);
	strcat(gMes, Mes);
	free(Name);
	free(Domain);

	// 5. DACL 정보 읽기 
	//GetSecurityInfo()로부터 DACl 포인터를 얻었다.
	// pDacl에 정보는 들어와 있다.
	// ACE의 개수 얻기 
	GetExplicitEntriesFromAcl(pDacl, &nAce, &pEntry);
	wsprintf(Mes, "ACE의 개수 = %d\n", nAce);
	strcat(gMes, Mes);

	for(i=0;i<(int)nAce; i++)
	{
		cbName = 0;
		cbDomain = 0;
		LookupAccountSid(NULL, pEntry[i].Trustee.ptstrName, NULL, &cbName,
			NULL, &cbDomain, &peUse);
		Name = (char*)malloc(cbName);
		Domain = (char*)malloc(cbDomain);
		LookupAccountSid(NULL, pEntry[i].Trustee.ptstrName, Name, &cbName,
			Domain, &cbDomain, &peUse);	

		wsprintf(Mes, "%s사용자에게 %x 액세스 권한을 %s한다.\n",
			Name, pEntry[i].grfAccessPermissions,
			pEntry[i].grfAccessMode == GRANT_ACCESS ? "허가" : "거부");
		strcat(gMes, Mes);
		free(Name);
		free(Domain);
	}

	// 메모리 해제 결과 출력
	LocalFree(pEntry);
	LocalFree(pSD);
	InvalidateRect(NULL, NULL, TRUE);

}
// 메세지 처리함수.


LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch( msg )
	{
	case WM_LBUTTONDOWN:
		GetSecurity();
		return 0;
	case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			RECT rt;
			GetClientRect(hWnd, &rt);
			DrawText(hdc, gMes, -1, &rt, 0);
			EndPaint(hWnd, &ps);
			break;

	case WM_DESTROY: 
		PostQuitMessage(0); 
		return 0;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam);
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

	MSG msg;
	while ( GetMessage( &msg, 0, 0, 0) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}



