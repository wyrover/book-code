#pragma comment(linker, "/subsystem:windows")

#include <windows.h>
#include "resource.h"

// DialogBoxParam() 을 사용하기 위해서는 구조체가 필요하다.
typedef struct _DLGDATA
{
	int x;
	int y;
} DLGDATA, *PDLGDATA;


// 사용자가 만드는 다이얼로그 메세지 처리 함수
BOOL CALLBACK DlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static DLGDATA* pData;
	switch( msg )
	{
	case WM_INITDIALOG: // Dialog 가 나타날때 나오는 메세지
		pData = (DLGDATA*)lParam; // DialogBoxParam()의 마지막 인자

		// 구조체의 내용으로 control을 초기화 한다
		SetDlgItemInt( hDlg, IDC_EDIT1, pData->x, 0);
		SetDlgItemInt( hDlg, IDC_EDIT2, pData->y, 0);
		return TRUE;

	case WM_COMMAND:    // 버튼(자식)을 누를때 처리
		switch( LOWORD( wParam ) )
		{
		case IDOK:
			{
				BOOL b;
				// Control 값 -> 구조체로..!!
				pData->x = GetDlgItemInt( hDlg, IDC_EDIT1, &b, 0);
				pData->y = GetDlgItemInt( hDlg, IDC_EDIT2, &b, 0);

				EndDialog( hDlg, IDOK);
			}
			break;

		/*
		case IDOK:   
			{
				char s[256];

				// ID  -> HWND을 얻는다.
				HWND hEdit = GetDlgItem( hDlg, IDC_EDIT1 );
				GetWindowText( hEdit, s, 256 );

				// 위 2줄대신 아래 한줄로도 가능
				// GetDlgItemText( hDlg, IDC_EDIT1, s, 256);
				
				MessageBox( 0, s, "", MB_OK);

				EndDialog( hDlg, IDOK );   
			}
			break;
			*/


		case IDCANCEL:EndDialog( hDlg, IDCANCEL); break;
		}
		return TRUE;
	}
	return FALSE; // 메세지를 처리하지 않은 경우.
}



LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int xPos = 0, yPos = 0;
	switch( msg )
	{	
	case WM_LBUTTONDOWN: 
		{
			// DialogBox를 나타내기전에 구조체를 생성해서 static 변수값으로 초기화 
			DLGDATA data;
			data.x = xPos;
			data.y = yPos;

			// 모달 다이얼로그 나타내기.
			UINT ret = DialogBoxParam( GetModuleHandle( 0 ), // hInstance
								MAKEINTRESOURCE(IDD_DIALOG1), // 리소스 ID
								hwnd,		// 소유자 핸들.
								DlgProc,    // 다이얼로그 프로시져.
								(LPARAM)&data); 
							// WM_INITDIALOG의 lParam으로 전달

			if ( ret == IDOK)
			{
				// Dialog에 입력한 값은 구조체에 있다.
				// Static 변수에 옮겨 담고 무효화
				xPos = data.x;
				yPos = data.y;
				InvalidateRect( hwnd, 0, TRUE);
			}
		}
		return 0;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint( hwnd, &ps );
			char s[256];
			wsprintf( s, "%d, %d", xPos, yPos );

			TextOut( hdc, 10, 10, s, strlen(s) );
			EndPaint( hwnd, &ps );
		}
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

	atom = RegisterClass( &wc);
	
	if ( atom == 0 ) 
	{
		MessageBox( 0, "Fail To RegisterClass", "Error", MB_OK);
		return 0;
	}

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


// API 정복 : 26장.  34장, 36장, 54장-- 기본
//           39~45 : 아주 중요  
//           49, 51: 좀 어려운 내용(시간상 다루지 못할수도 있음)




















