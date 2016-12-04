#include "Form.h"
#include "GlobalResource.h"

Form::Form(bool isMainForm, WCHAR * caption)
{
	m_isMainForm = isMainForm;
	TCHAR className[] = TEXT ("Form") ;
	HWND         hwnd ;
	MSG          msg ;
	WNDCLASS     wndclass ;

	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = WndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = applicationHandle;
	wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = className ;
	WNDCLASS tmpClass;
	if (!GetClassInfo(applicationHandle, className, &tmpClass))
	{
		if (!RegisterClass (&wndclass))
		{
			return;
		}
	}
	m_handle = CreateWindow (className, caption,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, applicationHandle, NULL) ;
	addWindowHandle(m_handle, this);
	ShowWindow (m_handle, SW_SHOW) ;
	UpdateWindow (m_handle);
}

LRESULT CALLBACK Form::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Form * self = (Form *)getObject(hwnd);
	if (self == NULL)
	{
		return DefWindowProc (hwnd, message, wParam, lParam);
	}
	switch (message)
	{	
	case WM_COMMAND :
		break ;

	case WM_DESTROY:
		delete self;
		return 0 ;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}

Form::~Form()
{
	delWindowHandle(m_handle);
	DestroyWindow(m_handle);	
	if (m_isMainForm)
		PostQuitMessage(0);
}
