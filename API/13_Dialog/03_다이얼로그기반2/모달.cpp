#include <windows.h>		
#include "resource.h"		
#include <CommCtrl.h>


#pragma comment(lib, "ComCtl32.lib")

// Dialog에서 값을 얻기 위한 구조체
struct DATA
{
	int cx;		int cy;		int weight;
};


HWND g_hDlg; // Dialog 의 핸들
////////////////////////////////////////////////////////////////////////
// Dialog Box Procedure..
// 이함수는 진짜 메세지 함수는 아니다. 진짜 메세지 함수는 user32.dll에
// 있는 DefDlgProc이다. DefDlgProc이 다시 아래 함수를 호출하는 것이다.
BOOL CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hHorz;		static HWND hVert;		static HWND hSlider;
	static DATA* pData;
	switch(msg)
	{
	// Dialog가 처음 나타날때 발생. 각 콘트롤을 초기화 한다.
	case WM_INITDIALOG:
		{
			pData = (DATA*)lParam;
			// pData의 값으로 Dialog를 초기화 한다.
			SetDlgItemInt( hDlg, IDC_EDIT_HORZ, pData->cx, FALSE);
			SetDlgItemInt( hDlg, IDC_EDIT_VERT, pData->cy, FALSE);
			// 각 Control을 초기화 한다.
			hHorz = GetDlgItem(hDlg, IDC_SPIN_HORZ);
			hVert = GetDlgItem(hDlg, IDC_SPIN_VERT);
			hSlider = GetDlgItem(hDlg, IDC_SLIDER_WEIGHT);

			// 각 Control에 메세지 전달.
			SendMessage( hHorz, UDM_SETRANGE32, 0, 300);
			SendMessage( hVert, UDM_SETRANGE32, 0, 300);

			SendMessage( hSlider, TBM_SETRANGE, TRUE, MAKELONG(0,8));
			SendMessage( hSlider, TBM_SETPOS, TRUE,pData->weight);
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK: 
			{
				// Control의 값을 읽어서 pData에 넣는다.
				BOOL b;
				pData->cx = GetDlgItemInt(hDlg, IDC_EDIT_HORZ, &b, 0);
				pData->cy = GetDlgItemInt(hDlg, IDC_EDIT_VERT, &b, 0);
				pData->weight = SendMessage( hSlider, TBM_GETPOS,0,0);
			}
			EndDialog(hDlg, IDOK); 
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
	}
	return FALSE; // 메세지 처리를 안한경우..
}

/////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE s_hInst;
	static int s_nWeight;	
	static int s_cx, s_cy;	// block 수
	
	switch(msg)
	{
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDM_OPTION_SETTING:
			{
				// Dialog를 나타내기 전에 구조체를 초기화.
				DATA data;
				data.cx = s_cx;
				data.cy = s_cy;
				data.weight = s_nWeight;

				// DialogBox 나타내기. 리턴 값은 EndDialog의 2번째 파라미터.
				UINT ret = DialogBoxParam( s_hInst,// hinstance
									MAKEINTRESOURCE(IDD_SETTING),
					hwnd,		// 부모
					DlgProc, // 메세지 함수.
					(LPARAM)&data); // WM_INITDIALOG의 lParam
									// 로 전달된다.
				if ( ret == IDOK ) 
				{
					// 이제 Dialog에 입력한 값은 data에 있다.
					s_cx = data.cx;
					s_cy = data.cy;
					s_nWeight = data.weight;

					InvalidateRect(hwnd, 0, TRUE);
				}
			}
		}
		return 0;

/////////////////////////////////////////////////////////////////////////////////
	case WM_CREATE:
		InitCommonControls(); // 공용 Control 초기화...
		s_hInst = ((LPCREATESTRUCT)lParam)->hInstance;

		s_nWeight = 0; // 0 ~ 8 색 0: 검정. 8 : 하얀색.
		s_cx = 8;
		s_cy = 8;
		return 0;

	// Draw s_cx * s_cy Grid.... color s_nWeight :0 -> black / s_nWeight : 8 -> white
	case WM_PAINT:
		{ 
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
	
			RECT r;
			GetClientRect(hwnd, &r);
			int nShade = s_nWeight * 32;
			if (nShade != 0) nShade--; // ??

			HPEN hPen = CreatePen(PS_SOLID, 1, RGB(nShade, nShade, nShade));
			HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

			int x;
			for (int i = 1; i < s_cx ; i++)
			{
				x = (r.right *i) / s_cx;
				MoveToEx(hdc, x,0, NULL);
				LineTo(hdc, x, r.bottom );
			}
			int y;
			for (i = 1; i < s_cy ; i++)
			{
				y = (r.bottom * i) / s_cy;
				MoveToEx(hdc, 0,y, NULL);
				LineTo(hdc, r.right , y);
			}

			DeleteObject( SelectObject(hdc, hOldPen));
			EndPaint(hwnd, &ps);
		}
		return 0;

	case WM_DESTROY:
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
	wc.lpszMenuName			= MAKEINTRESOURCE(IDR_MAINFRAME);
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