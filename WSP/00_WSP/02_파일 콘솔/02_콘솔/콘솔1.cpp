/**************************************************************************

[ GUI 프로그램에서의 콘솔 사용 ]

1) 콘솔 명시적으로 생성
   AllocConsole()
     - 인  자 : 없슴
     - 반환값 : 콘솔 생성의 성공 여부 반환


2) 콘솔의 할당 해제
   FreeConsole()
     - 인  자 : 없슴

    *) 여러 개의 프로그램이 콘솔 하나를 공유할 경우 할당 해제 
       공유하지 않을경우는 파기 

3) 콘솔화면에 출력
   출력용 버퍼 핸들 얻기 : 콘솔에서는 입력용, 출력용 버퍼가 각각 준비되어 있다.
     특히 출력용 버퍼를 스크린 버퍼하고 함
     버퍼는 Win32 API를 사용해서 마치 파일처럼 취급할수 있다. 
     버퍼에 읽기 / 쓰기를 하는 것은 콘솔에서의 입출력과 같다.

   - 버퍼 핸들을 얻어 올 경우 
     입력 : CONIN$
     출력 : CONOUT$ 
     위의 특수 파일명을 지정해서 파일 생성용 API 인 CreateFile 을 호출함

   - 입출력
      ReadFile 과 WriteFile을 사용해서 입출력 수행 할 수 있다.
      *) GUI에서 콘솔 입력시 Windows에서 통지되는 것이 아니라 프로그램이 읽으러
         가는 것임, 따라서 입력이 있을 때까지 리턴이 안됨
         즉, 그때까지 GUI  프로그램의 메시지 루프는 멈추어짐 


[ 콘솔 API를 사용 ]

C/C++ 라이브러리 함수는 운영체제간 호환성이 높은 프로그램을 만들 수 있다는 장점이 있지만,
기능이 너무 빈약하거나 좀더 자유롭게 화면 제어를 할 수 없는 단점이 있다.

Windows	 콘솔은 콘솔 함수라고 하는 일련의 Win32API에서 지원하는 강력한 제어 기능을 갖추고 있다.
  ex) AllocConsole / FreeConsole


  *) 스크린 버퍼 
     콘솔 윈도우의 화면 출력 내용을 보관 유지해서 메모리의 가상 화면 역할을 수행한다.
     실체는 출력하려는 문자 정보를 저장한 2차원 배열임
     이 배열은 가상 화면에 출력할 수 있는 '가로 문자 수 * 세로 행 수' 크기만큼 요소를 포함하며,
     각 요소에 출력하려는 문자의 문자 코드와 문자색 / 배경색 등의 속성을 CHAR_INFO 타입의 구조체로 저장함

     스크린 버퍼가 나타내는 가상 화면의 크기가 콘솔 윈도우보다 커도 상관없다. 
     그런 경우에는 가상 화면의 일부 영역만이 콘솔에 표시된다.
     이 때 Windows에서는 콘솔 윈도우에 스크롤바를 추가한다.

     콘솔에 문자를 출력하려면 출력용 함수를 사용해서 스크린 버퍼의 특정 위치에 문자 정보를 써넣는다.
     이 함수는 버퍼를 추상화해서 다루며, 버퍼를 직접 조작할 수 없는 경우와 자유롭게 조작할 수 있는 경우로 
     나눌 수 있다. 전자를 고수준 API, 후자를 저수준 API라고 한다.


  *) 커서의 위치 
     호출 후에 자동으로 쓰기 종료된 위치로 이동한다.
     SetConsoleCursorPosition : 커서의 위치를 자유롭게 변경할 수 있다.
     SetConsoleTextAttribute  : 문자색을 자유롭게 변경 출력가능하다.
*************************************************************************/

#include <windows.h>
#include <windowsx.h>		// GET_X_LPARAM


LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_CREATE:

		// 콘솔 생성 
		if(!AllocConsole() ) return -1;

		return 0;
	case WM_LBUTTONDOWN:
		{
			// "CONOUT$ : 출력용 버퍼의 핸들을 얻어옴...
			// "CONIN$" : 입력용 버퍼의 핸들을 얻어옴...
			//  - GUI에서는 보통 디버깅용으로 사용함 
			HANDLE hOut = CreateFile("CONOUT$", 
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL, OPEN_EXISTING, 0, 0);
			if( hOut != INVALID_HANDLE_VALUE)
			{
				char buf[128];
				DWORD sz;

				// 문자열을 콘솔에 출력
				wsprintf(buf, "(%d : %d) 클릭\n", 
					GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

				WriteFile(hOut, buf, strlen(buf), &sz, NULL);
				CloseHandle(hOut);
			}
			else
			{
				MessageBox(hwnd, "콘솔에 기록 실패", "ERROR", MB_OK | MB_ICONEXCLAMATION);
			}
		}
		return 0;
	case WM_DESTROY:

		// 콘솔 할당 해제 
		FreeConsole();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nShowCmd)
{
	// 1. 윈도우 클래스 만들기 
	WNDCLASS wc;
	wc.cbWndExtra			= 0;
	wc.cbClsExtra			= 0;
	wc.hbrBackground		= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor				= LoadCursor(0, IDC_ARROW);
	wc.hIcon				= LoadIcon(0, IDI_APPLICATION);
	wc.hInstance			= hInstance;
	wc.lpfnWndProc			= WndProc;			// DefWindowProc;
	wc.lpszClassName		= "First";
	wc.lpszMenuName			= 0;
	wc.style				= 0;

	// 2. 등록(레지스트리에)
	RegisterClass(&wc);

	// 3. 윈도우 창 만들기 
	HWND hwnd = CreateWindowEx( 0,					// WS_EX_TOPMOST
				 "first",				// 클래스 명
				 "Hello",				// 캡션바 내용
				 WS_OVERLAPPEDWINDOW, 
				 CW_USEDEFAULT , 0, CW_USEDEFAULT, 0,	// 초기 위치 
				 0, 0,			// 부모 윈도우 핸들, 메뉴 핸들
				 hInstance,		// WinMain의 1번째 파라미터 (exe 주소)
				 0);			// 생성 인자



	// 4. 윈도우 보여주기
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	

	// 5. Message 
	MSG msg;
	while( GetMessage( &msg, 0, 0, 0 ) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}