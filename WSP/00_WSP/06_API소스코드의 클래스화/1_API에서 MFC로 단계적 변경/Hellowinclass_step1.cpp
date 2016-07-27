/*******************************************************************

  GUI에서 메시지 처리 방식 
   - EVENT발생 => Message Queue에 전송 => Message Proc에 전송

  MFC에서 사용하는 메시지 처리 방식
  가상함수를 사용하지 않고 매시지 맵을 통한 구현
*******************************************************************/


//=================================================================
//   01단계 ) 클래스 사용 
//    
//    - WinMain의 구분
//		1) 윈도우 클래스 등록, 윈도우 생성	: 인스턴스 초기화
//		2) 메시지 루프						: 메시지 루프 
//		3) 메시지 루프를 탈출한 뒤의 종료 처리 : 인스턴스 탈출 
//=================================================================

#include <windows.h>

//메시지 처리 함수 선언 
LRESULT CALLBACK WndProc(HWND hwnd,UINT iMsg,WPARAM wParam, LPARAM lParam);


//클래스의 정의 
class CApp 
{
    static char szAppName[];
    HWND        hwnd;
    MSG         msg;
    WNDCLASSEX  wndclass;

public:

	// 초기화 : 레지스트리 등록 및 window 생성
    void		InitInstance(HINSTANCE hInstance, PSTR szCmdLine, 
                      int iCmdShow);

	// 실  행 : 메시지 루프 
    void		Run();

	// 종  료 : 종료 처리 
    WPARAM		ExitInstance();


    // message handler
    void OnCreate();
    void OnDraw();
    void OnDestroy();
};


void CApp::InitInstance(HINSTANCE hInstance,PSTR szCmdLine,
                        int iCmdShow) {

    wndclass.cbSize         = sizeof(wndclass);
    wndclass.style          = CS_HREDRAW|CS_VREDRAW;
    wndclass.lpfnWndProc    = WndProc;
    wndclass.cbClsExtra     = 0;
    wndclass.cbWndExtra     = 0;
    wndclass.hInstance      = hInstance;
    wndclass.hIcon          = LoadIcon(NULL,IDI_APPLICATION);
    wndclass.hCursor        = LoadCursor(NULL,IDC_ARROW);
    wndclass.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName   = NULL;
    wndclass.lpszClassName  = szAppName;
    wndclass.hIconSm        = LoadIcon(NULL,IDI_APPLICATION);

    RegisterClassEx(&wndclass);

    hwnd=CreateWindow(szAppName,    //window class name
        "The Hello Program",        //window caption
        WS_OVERLAPPEDWINDOW,        //window style
        CW_USEDEFAULT,              //initial x position
        CW_USEDEFAULT,              //initial y position
        CW_USEDEFAULT,              //initial x size
        CW_USEDEFAULT,              //initial y size
        NULL,                       //parent window handle
        NULL,                       //window menu handle
        hInstance,                  //program instance handle
        NULL);                      //creation parameters

    ShowWindow(hwnd,iCmdShow);
    UpdateWindow(hwnd);
}



void CApp::Run() 
{
    while (GetMessage(&msg,NULL,0,0)) 
	{
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}



WPARAM CApp::ExitInstance() 
{
    return msg.wParam;
}



char CApp::szAppName[]="HelloWin";


//Event handler------------------------------------------------------
void CApp::OnCreate() {	}

void CApp::OnDraw() 
{
    HDC         hdc;
    PAINTSTRUCT ps;
    RECT        rect;

    hdc=BeginPaint(hwnd,&ps);
    GetClientRect(hwnd,&rect);
    DrawText(hdc,"Hello, Windows!",-1,&rect,
        DT_SINGLELINE|DT_CENTER|DT_VCENTER);
    EndPaint(hwnd,&ps);
}


void CApp::OnDestroy() 
{
    PostQuitMessage(0);
}


//Global object------------------------------------------------------
CApp app;


//Window procedure---------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd,UINT iMsg,WPARAM wParam,
                         LPARAM lParam) 
{
    switch (iMsg) 
	{
    case WM_CREATE:
        app.OnCreate();    
        return 0;
    case WM_PAINT:
        app.OnDraw();
        return 0;
    case WM_DESTROY:
        app.OnDestroy();
        return 0;
    }
    return DefWindowProc(hwnd,iMsg,wParam,lParam);
}




int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
                   PSTR szCmdLine,int iCmdShow) 
{
    app.InitInstance(hInstance,szCmdLine,iCmdShow);

    app.Run();

    return app.ExitInstance();
}


/*
처리해야 할 메시지 발생시 마다 맴버 함수 구현 필요
그 때마다 윈도우 프로시저를 수정하는 것은 바람직 하지 않음
따라서 WinMain과 윈도우 프로시저를 최종 구현자에게 완전히 숨기기를 원함

최종 구현자는 자신이 처리해야 할 메시지에 집중해서 프로그래밍을 하게 됨..
*/