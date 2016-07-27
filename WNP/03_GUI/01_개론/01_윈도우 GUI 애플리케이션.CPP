//------------------------------------------------------------
// 윈도우 GUI 애플리케이션의 구조와 동작 원리 이해 
//------------------------------------------------------------

/*
//------------------------
//1. 구조와 동작 원리 
//------------------------

  다양한 API를 이용하여 편리하고 화려한 사용자 인터페이스를 제공

  핵심적 특징은 메시지 구동 구조(message-driven architecture)
   - 메시지란 ? 애플리케이션의 외부 또는 내부에서 어떤 변화가 발생
     했음을 해당 애플리케이션에 알리기 위해 운영체제가 제공하는
	 개념

  -  이벤트(event :사건)  => 
     시스템 메시지 큐(system message queue)에 정보 저장 : 운영체제가 관리

     애플리케이션 메시지 큐(application message queue)  : 
	    각각의 GUI 애플리 케이션은 운영체제로부터 독립적인 메시지 큐 할당 
		해당 애플리케이션의 메시지 큐로 전송

     메시지 감시 
	    GUI 애플리케이션은 메시지 큐를  감시(getmessage or peekmessage)
		하고 있다가 메시지가 발생해서 큐에 들어오면 이를 하나씩 꺼내서 
		적절히 처리 , 메시지가 없을 경우 대기 함 

     * 메시지 큐는 FIFO(First-in-First-out) 방식으로 동작함 
	     먼저 들어간 메시지가 먼저 빠져나오는 방식
         

     메시지 프로시져 
	    메시지 핸들러(message handler) : 키보드, 마우스, 메뉴 등의 메시지 핸들러 
		메시지 핸들러들의 집합을 메시지 프로시져라 부른다.

        프로그래머가 작성한 메시지가 아닌 경우 운영체제가 자동으로 처리하도록 한다.



// 1) 윈도우 API 함수 원형과 각종 데이터 타입 매크로 상수 등을 포함하는 헤더 파일
#include <windows.h>	


// 2) 윈도우 프로시저 사용자 정의 함수 원형 선언 
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)


// 3) 콘솔 어플리케이션의 main()함수에 해당 
//    인스턴스핸들, ___, 명령행 인자, 프로그램 시작할 때 윈도우 모양 
//    인스턴스 핸들은 실행 파일에 포함된 각종 리소스를 접근할 때 일반적으로 사용
//    얻는 방법 ??
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nShowCmd)
{
	// 4) 윈도우 클래스 등록 
	//    윈도우의 다양한 특성을 정의한 구조체 
	//    SetClassLong/GetClassLong 함수로 변경 가능 ...
	//    레지스트리에 등록해 두면 여러 개의 윈도우를 생성할 수 있다. 
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
	RegisterClass(&wc);


	// 5) 윈도우 창 생성
	//    윈도우의 핸들 리턴 => 다른 API 함수 호출시 사용
	//    
	HWND hwnd = CreateWindowEx( 0,					// WS_EX_TOPMOST
				 "first",				// 클래스 명
				 "Hello",				// 캡션바 내용
				 WS_OVERLAPPEDWINDOW, 
				 CW_USEDEFAULT , 0, CW_USEDEFAULT, 0,	// 초기 위치 
				 0, 0,			// 부모 윈도우 핸들, 메뉴 핸들
				 hInstance,		// WinMain의 1번째 파라미터 (exe 주소)
				 0);			// 생성 인자



	// 6) 윈도우 보여주기
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	

	// 7) Message  loop
	//    메시지 큐에서 메시지를 꺼내서 
	//    키보드 메시지를 처리한 후 
	//    해당 메시지를 윈도우 프로시저에 전달 
	MSG msg;
	while( GetMessage( &msg, 0, 0, 0 ) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
     

// 8) 윈도우 메시지를 처리하는 함수인 윈도우 프로시져 
//    처리하지 않는 메시지는 운영체제가 알아서 처리하도록 한다. 
//    hwnd : 메시지가 발생한 윈도우를 나타내는 핸들값 
//    msg  : 발생한 메시지 일반적으로 WM_* 형태의 상수값으로 정의된다.
//    wParam, lParam : 메시지와 더불어 전달되는 부가 정보 각각 32bit 크기 
//                     MSDN 참조..
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_LBUTTONDOWN:
		return 0;
	case WM_RBUTTONDOWN:
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
   
       
     
   * 컨트롤 
     일반적으로 대화상자에서 사용
	 어떤 경우든 독립적인 윈도우가 아닌 자식 윈도우(child window)로 존재함
	 컨트롤의 부모 윈도우(parent window)는 SendMessage() 함수를 이용하여 
	 컨트롤에 메시지를 보낼 수 있다. 


*/



//  [ 실  습 ]

#include <windows.h>
#include <stdio.h>

// 윈도우 메시지 처리 함수
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
// 에디트 컨트롤 출력 함수
void DisplayText(char *fmt, ...);

HINSTANCE hInst; // 인스턴스 핸들
HWND hEdit; // 에디트 컨트롤

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE,
	LPSTR lpCmdLine, int nCmdShow)
{
	hInst = hInstance;

	// 윈도우 클래스 등록
	WNDCLASS wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpszClassName = "MyWindowClass";
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = (WNDPROC)WndProc;
	if(!RegisterClass(&wndclass)) return -1;

	// 윈도우 생성
	HWND hWnd = CreateWindow("MyWindowClass", 
		"WinApp", WS_OVERLAPPEDWINDOW,
		0, 0, 600, 300,
		NULL, (HMENU)NULL, hInstance, NULL);
	if(hWnd == NULL) return -1;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// 메시지 루프
	MSG msg;
	while(GetMessage(&msg, 0, 0, 0) > 0){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

// 윈도우 메시지 처리
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam)
{
	switch(uMsg){
	case WM_CREATE:
		hEdit = CreateWindow("edit", NULL, 
			WS_CHILD | WS_VISIBLE | WS_HSCROLL | 
			WS_VSCROLL | ES_AUTOHSCROLL | 
			ES_AUTOVSCROLL | ES_MULTILINE,
			0, 0, 0, 0, hWnd, (HMENU)100, hInst, NULL);

		DisplayText("GUI 소켓 애플리케이션을 위한 예제입니다.\r\n");
		DisplayText("인스턴스 핸들값은 0x%x입니다.\r\n", hInst);

		return 0;
	case WM_SIZE:
		MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		Sleep(3000);
		return 0;
	case WM_SETFOCUS:
		SetFocus(hEdit);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void DisplayText(char *fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);
	
	char cbuf[256];
	vsprintf(cbuf, fmt, arg);
	
	int nLength = GetWindowTextLength(hEdit);
	SendMessage(hEdit, EM_SETSEL, nLength, nLength);
	SendMessage(hEdit, EM_REPLACESEL, FALSE, (LPARAM)cbuf);
	
	va_end(arg);
}
