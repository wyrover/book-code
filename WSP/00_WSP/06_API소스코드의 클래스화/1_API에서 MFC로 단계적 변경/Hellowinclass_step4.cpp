//=================================================================
//   04단계 ) 매크로 사용
//            반복 코드를 간단하게 표현하고, 코드 생성을 쉽게 함 
//=================================================================

/*
최종 결론)

 1) 이미 만들어진 WinMain()과 WinProc(), CObject를 사용할 수 있다.
 2) 최종 사용자는 CObject를 상속받는 자신의 클래스를 정의한 다음
    처리를 원하는 메시지에 대해서만 다음의 네 가지를 한다.
	a) 정적 구조체 메시지 맵의 선언 
	b) 처리를 원하는 메시지 핸들러의 선언
	c) 정적 구조체 메시지 맵의 초기화
	d) 선언된 메시지 핸들러의 구현 

*/

#include <windows.h>

#define DECLARE_MESSAGE_MAP()          static MessageMap messageMap[];

#define BEGIN_MESSAGE_MAP(class_name)  MessageMap class_name::messageMap[]={

#define END_MESSAGE_MAP()              {0,NULL}};


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
};

void CObject::InitInstance(HINSTANCE hInstance,PSTR szCmdLine,
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

    hwnd = CreateWindow(szAppName,    //window class name
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

void CObject::Run() {
    while (GetMessage(&msg,NULL,0,0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

WPARAM CObject::ExitInstance() {
    return msg.wParam;
}

char CObject::szAppName[]="HelloWin";




// MessageMap, It's punch line! ----------------------------------
class CView;

typedef void (CView::*CViewFunPointer)();

typedef struct tagMessageMap 
{
    UINT iMsg;
    CViewFunPointer fp;
} MessageMap;

static CViewFunPointer fpCViewGlobal;//pointer to a member function




// class CView----------------------------------------------------
class CView : public CObject 
{
public:
    // 1) 메시지 핸들러의 선언(특수한 주석 안에서 처리 )
    //{{AFX_MESSAGEMAP
    void OnCreate();
    void OnDraw();
    void OnDestroy();
    //}}AFX_MESSAGEMAP


    // 2) 메시지를 저장할 static 배열 선언 

    DECLARE_MESSAGE_MAP()
	//static MessageMap messageMap[];   로 확장 
};



// 3) 메시지를 저장할 배열에 저장 
//MessageMap class_name::messageMap[]={
BEGIN_MESSAGE_MAP(CView)
    {WM_CREATE,CView::OnCreate},
    {WM_PAINT,CView::OnDraw},
    {WM_DESTROY,CView::OnDestroy},
END_MESSAGE_MAP()
//{0,NULL}};



//CView Event handler------------------------------------------------
void CView::OnCreate() {  }


void CView::OnDraw() 
{
    HDC         hdc;
    PAINTSTRUCT ps;
    RECT        rect;

    hdc = BeginPaint(hwnd,&ps);

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
                         LPARAM lParam) 
{
    int i=0;

    while ( 0 != CView::messageMap[i].iMsg ) 
	{
        if ( iMsg == CView::messageMap[i].iMsg ) 
		{
            fpCViewGlobal = CView::messageMap[i].fp;
            (app.*fpCViewGlobal)();
            return 0;
        }
        ++i;
    }
    return DefWindowProc(hwnd,iMsg,wParam,lParam);
}


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
                   PSTR szCmdLine,int iCmdShow) {
    app.InitInstance(hInstance,szCmdLine,iCmdShow);
    app.Run();
    return app.ExitInstance();
}