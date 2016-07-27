//=================================================================
//   02단계 ) 가상함수 사용 
//             WinMain과 윈도우 프로시저 숨기기

//   CObject 
//   CView
//   => CObject(부모)   CView(자식)

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

    // 순수 가상 함수 
    virtual void OnCreate()      = 0;
    virtual void OnDraw()        = 0;
    virtual void OnDestroy()     = 0;
    virtual void OnLButtonDown() = 0;
};



void CObject::InitInstance(HINSTANCE hInstance,PSTR szCmdLine,
                        int iCmdShow) 
{
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


//----------------------------------------------------------------
// pointer declaration for upcase(subtype principle)
CObject* pCObject; 



//class CView--------------------------------------------------------
class CView : public CObject 
{
public:
    // 순수 가상 함수 재정의 
    void OnCreate();
    void OnDraw();
    void OnDestroy();
    void OnLButtonDown();
};

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


void CView::OnLButtonDown() 
{
    MessageBox(NULL,"MESSAGE","TITLE",MB_ICONEXCLAMATION|MB_OK);
}



//Global object------------------------------------------------------
CView app;



//Window procedure---------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd,UINT iMsg,WPARAM wParam,
                         LPARAM lParam) 
{
    switch (iMsg) {
    case WM_CREATE:
        pCObject->OnCreate();    
        return 0;
    case WM_PAINT:
        pCObject->OnDraw();
        return 0;
    case WM_DESTROY:
        pCObject->OnDestroy();
        return 0;
    case WM_LBUTTONDOWN:
        pCObject->OnLButtonDown();
        return 0;
    }//switch
    return DefWindowProc(hwnd,iMsg,wParam,lParam);
}



int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
                   PSTR szCmdLine,int iCmdShow) 
{
    pCObject = &app;
    pCObject->InitInstance(hInstance,szCmdLine,iCmdShow);
    pCObject->Run();
    return pCObject->ExitInstance();
}

/*

  1) WinMain과 WinProc을 자동화 할 수 있다.
  2) 사용자에 의해 지정된 클래스 이름으로 생성된 전역 객체의 시작 주소를
     이 포인터에 대입하면, WinMain과 윈도우프로시저를 최종 사용자에게 
	 숨길 수 있다.

  3) 200개가 넘는 메시지에 대응하는 가상 함수가 존재해야 한다는 것이 문제

  4) 추가되는 메시지와 사용자 정의 메시지는 처리할 수 없다.

*/