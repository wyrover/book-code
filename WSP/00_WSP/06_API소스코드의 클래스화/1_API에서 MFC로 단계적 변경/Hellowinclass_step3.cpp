//=================================================================
//   03단계 ) 맴버 함수 포인터 테이블의 이용 
//			 - 추가되는 메시지에 대한 자동화도 가능 
//			 - 기본 원리 : 메시지와 핸들러를 하나의 배열에 저장
//				메시지 맵이라 함 
//=================================================================


#include <windows.h>

//Forward declaration------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd,UINT iMsg,WPARAM wParam,
                         LPARAM lParam);



//Class CObject------------------------------------------------------
class CObject 
{
protected:
    static char szAppName[];
    HWND        hwnd;
    MSG         msg;
    WNDCLASSEX  wndclass;
public:

    void InitInstance(HINSTANCE hInstance,PSTR szCmdLine,
                      int iCmdShow);

    void Run();

    WPARAM ExitInstance();

	// 메시지 관련 순수 가상함수가 없어짐 
};


void CObject::InitInstance(HINSTANCE hInstance,PSTR szCmdLine,
                        int iCmdShow) {
    wndclass.cbSize         =sizeof(wndclass);
    wndclass.style          =CS_HREDRAW|CS_VREDRAW;
    wndclass.lpfnWndProc    =WndProc;
    wndclass.cbClsExtra     =0;
    wndclass.cbWndExtra     =0;
    wndclass.hInstance      =hInstance;
    wndclass.hIcon          =LoadIcon(NULL,IDI_APPLICATION);
    wndclass.hCursor        =LoadCursor(NULL,IDC_ARROW);
    wndclass.hbrBackground  =(HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName   =NULL;
    wndclass.lpszClassName  =szAppName;
    wndclass.hIconSm        =LoadIcon(NULL,IDI_APPLICATION);

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

void CObject::Run() 
{
    while (GetMessage(&msg,NULL,0,0)) 
	{
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

WPARAM CObject::ExitInstance() 
{
    return msg.wParam;
}

char CObject::szAppName[]="HelloWin";




//추가된 소스 코드 ---------------------------------------

class CView;

//CView 맴버 함수의 함수 포인터 재정의 (리턴 : void, 인자 : void)
typedef void (CView::*CViewFunPointer)();

typedef struct tagMessageMap 
{
    UINT iMsg;
    CViewFunPointer fp;
} MessageMap;

static CViewFunPointer fpCViewGlobal;
//추가된 소스 코드 ---------------------------------------



class CView : public CObject 
{
public:

	//   메시지 맵 배열 선언 
    static MessageMap messageMap[];
public:

	// 메시지 추가 
    void OnCreate();
    void OnDraw();
    void OnDestroy();
};


//{{BEGIN_MESSAGE_MAP
MessageMap CView::messageMap[]=
{
    {WM_CREATE,		CView::OnCreate},	// 메시지 id,  메시지 핸들러 
    {WM_PAINT,		CView::OnDraw},
    {WM_DESTROY,	CView::OnDestroy},
    {0,NULL}
};
//}}END_MESSAGE_MAP



//CView Event handler------------------------------------------------
void CView::OnCreate() {}


void CView::OnDraw()
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


void CView::OnDestroy() 
{
    PostQuitMessage(0);
}


//Global object------------------------------------------------------
CView app;



//Window procedure---------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd,UINT iMsg,WPARAM wParam,
                         LPARAM lParam) {
    int i=0;

    while (CView::messageMap[i].iMsg!=0) 
	{
        if (iMsg==CView::messageMap[i].iMsg) 
		{
            fpCViewGlobal=CView::messageMap[i].fp;
            (app.*fpCViewGlobal)();
            return 0;
        }
        ++i;
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

MFC의 메시지 맵 구조와 비슷한 맵 구조를 가짐..

WinMain은 자동화 되었다.
윈도우 프로시저도 자동화 되었다.

프로그래머는 실제 CView의 메시지 맵에만 집중하면 됨 

*/