#pragma comment(linker, "/subsystem:windows")

#include <windows.h>
void Draw3dRect(HDC hdc, int x, int y, int xx, int yy, 
					BOOL down, int width )
{
	COLORREF clrMain  = RGB(192,192,192),
				clrLight = RGB(255,255,255), 
				clrDark  = RGB(128,128,128);

	HPEN hPen1, hPen2, hOldPen;
	HBRUSH hBrush, hOldBrush;     

	if(down)    
	{
		hPen2 = CreatePen(PS_SOLID,1,clrLight);
		hPen1 = CreatePen(PS_SOLID,1,clrDark);
	} 
	else 
	{
		hPen1 = CreatePen(PS_SOLID,1,clrLight);
		hPen2 = CreatePen(PS_SOLID,1,clrDark);
	}  
	hBrush =  CreateSolidBrush( clrMain );
	hOldPen = (HPEN)SelectObject(hdc, hPen1);
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);  

	Rectangle(hdc, x , y, xx+1, yy+1);  

	for(int i=0; i < width; i++)
	{
		SelectObject(hdc, hPen1);

		MoveToEx(hdc, xx - 1, y, 0 );
		LineTo(hdc, x, y);
		LineTo(hdc, x, yy - 1 );  

		SelectObject(hdc, hPen2);

		MoveToEx(hdc, x, yy,0);
		LineTo(hdc, xx, yy);
		LineTo(hdc, xx, y);  

		x++; y++; xx--; yy--;
	} 
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);

	DeleteObject(hPen1); 
	DeleteObject(hPen2); 
	DeleteObject(hBrush);
} 

// 자식(버튼)이 부모에게 WM_COMMAND를 보낼때 사용할 통지코드(메세지를 보내는 이유)
#define BTN_LCLICK	1
#define BTN_RCLICK	2
#define BTN_LDBLCLK	3
#define BTN_RDBLCLK	4

// 부모가 자식(버튼)을 만들어 놓고 자식에게 보낼수 있는 메세지를 설계한다.
// 부모 - 자식 간의 약속.
#define BM_CHANGESTYLE	WM_USER + 10
#define BM_CHANGETHICK  WM_USER + 11

//========================================================================
// 자식용 메세지 처리 함수.
LRESULT CALLBACK ChildProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static BOOL bDown = FALSE;
	static int  nThick = 2;

	switch( msg )
	{	

	// 부모가 보내준 메세지
	case BM_CHANGETHICK:
		nThick = (int)wParam;  // wParam에 두께를 보내주기로 약속...
		InvalidateRect( hwnd, 0, FALSE);
		return 0;




	case WM_LBUTTONDOWN:
		bDown = TRUE;  InvalidateRect( hwnd, 0, FALSE );
		SetCapture( hwnd );
		return 0;

	case WM_LBUTTONUP:
		if ( GetCapture() == hwnd )
		{
			ReleaseCapture();
			bDown = FALSE; 
			InvalidateRect( hwnd, 0, FALSE );

			//========================================
			// 자신이 눌렸음을 부모에게 알려준다.
			HWND hParent = GetParent( hwnd );
			UINT id      = GetDlgCtrlID( hwnd );

			SendMessage( hParent,  WM_COMMAND, 
						MAKELONG(id, BTN_LCLICK),// 하위16:id, 상위16:통지코드
						(LPARAM)hwnd);
		}
		return 0;




	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint( hwnd, &ps );
			// 자식은 hdc 를 얻고 자신을 그리기 전에 부모에게 hdc 를 전달해준다.
			// 부모가 자식의 색상을 변경할 권한을 주기 위해서..
			// 이런 용도로 미리 만들어진 메세지가 WM_CTLCOLORxxx 이다.

			// wParam 에는 hdc 를 lParam 에는 자신의 핸들을 넣어 준다.
			HWND hParent = GetParent( hwnd );

			SendMessage( hParent, WM_CTLCOLORBTN, (WPARAM)hdc, (LPARAM)hwnd);

			//========================================================



			RECT rc;
			GetClientRect( hwnd, &rc);

			Draw3dRect( hdc, 0, 0, rc.right, rc.bottom, bDown, nThick );
			
			// 부모가 전달한 캡션 문자열을 윈도우 가운데 출력한다
			char s[256];
			GetWindowText( hwnd, s, 256 );

			SetBkMode( hdc, TRANSPARENT );
			
			if ( bDown == TRUE )
				OffsetRect( &rc, nThick, nThick ); // 오른쪽 아래로 사각형을
												// nThick 만큼 이동

			DrawText( hdc, s, -1, &rc, 
							DT_SINGLELINE | DT_CENTER | DT_VCENTER );
			//-----------------------------------------------------





			EndPaint( hwnd, &ps );
		}
		return 0;
			// user.chol.com/~downboard/mine.txt 에서 Draw3dRect() 복사해 오세요
	}
	return DefWindowProc( hwnd, msg, wParam, lParam);
}
		















//============================================================================
// 부모용 메세지 처리함수.
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hChild;

	switch( msg )
	{	
	// 자식이 자신을 그리기 전에 보내는 메세지 - 부모에게 색을 변경할 기회를 준다.
	case WM_CTLCOLORBTN:
		{
			HDC hdc = (HDC)wParam;
			HWND h  = (HWND)lParam;

			if ( hChild == h )  // 자식이 2개 이상이라면 내가 원하는 자식인지 확인
			{
				SetTextColor( hdc, RGB(255,0,0));
			}
		}
		return 0;






	case WM_LBUTTONDOWN:
		{
			static int n = 2;
			++n;

			// 자식에게 메세지를 보내서 두께를 변경하게 한다
			// wParam : 변경할 두께,   lParam : not used 
			SendMessage( hChild, BM_CHANGETHICK, n, 0);
		}
		return 0;



	// 자식(버튼)이 보내는 메세지
	case WM_COMMAND:
		switch( LOWORD(wParam) ) // 자식 ID
		{
		case 1:
			if ( HIWORD(wParam) == BTN_LCLICK ) // 통지코드 조사
			{
				MessageBox( hwnd, "Click!!", "", MB_OK);
			}
			break;
		}
		return 0;








	case WM_CREATE:

		hChild = CreateWindow( "CHILD", "자식", 
							WS_CHILD | WS_VISIBLE | WS_BORDER,
							10, 10, 100, 100,
							hwnd,	// 부모 윈도우 핸들
							(HMENU)1, // 자식은 메뉴를 가질수 없다. ID로 사용
							0, 0);
		return 0;





	case WM_DESTROY:
		PostQuitMessage(0); 
		return 0;
	}
	return DefWindowProc( hwnd, msg, wParam, lParam);
}







int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
					LPSTR	  lpCmdLine, int nShowCmd )
{
	ATOM atom;
	WNDCLASS wc;
	HWND hwnd;
	MSG msg;
	
	wc.cbClsExtra	= 0;
	wc.cbWndExtra	= 0;
	wc.hbrBackground= (HBRUSH)GetStockObject( WHITE_BRUSH ); 
	wc.hCursor		= LoadCursor( 0, IDC_ARROW );
	wc.hIcon		= LoadIcon( 0, IDI_APPLICATION);
	wc.hInstance	= hInstance;
	wc.lpfnWndProc  = WndProc; 
	wc.lpszClassName= "First";
	wc.lpszMenuName = 0;
	wc.style		= 0;

	atom = RegisterClass( &wc); // 부모용 윈도우 클래스 등록

	//========================================
	wc.lpfnWndProc = ChildProc;
	wc.lpszClassName = "CHILD";

	RegisterClass( &wc ); // 자식용 윈도우 클래스 등록
	//=========================================
	

	hwnd = CreateWindowEx( 0, "first", "Hello", WS_OVERLAPPEDWINDOW,
								CW_USEDEFAULT, 0, CW_USEDEFAULT,0, 0, 0, 
								hInstance, 0); 
	ShowWindow( hwnd, nShowCmd); 
	UpdateWindow( hwnd );

	while ( GetMessage( &msg, 0, 0, 0) ) 
	{							
		TranslateMessage(&msg);
		DispatchMessage( &msg);
	}

	return 0;
}

