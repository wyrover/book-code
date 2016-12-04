#include "Form.h"
#include "GlobalResource.h"

Form::Form(bool isMainForm, WCHAR * caption)
{
	m_isMainForm = isMainForm;
	m_destroyState = dsNo;
	TCHAR className[] = TEXT ("Form") ;
	HWND         hwnd ;
	MSG          msg ;
	WNDCLASS     wndclass ;
	LRESULT (__thiscall Form::* ptrFunc)(HWND,UINT,WPARAM,LPARAM);
	ptrFunc = (&Form::WndProc);
	void * func;
	__asm{
		mov eax, ptrFunc;
		mov func, eax;
	}
	m_wndProcStub = makeWndProcStub(this, func);	 
	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	///wndclass.lpfnWndProc   = initWndProc ;
	wndclass.lpfnWndProc   = DefWindowProc;
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
		//UnregisterClass(className, applicationHandle);
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
	SetWindowLong(m_handle, GWL_WNDPROC, (LONG)m_wndProcStub);
	ShowWindow (m_handle, SW_SHOW) ;
	UpdateWindow (m_handle);
}

LRESULT Form::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	switch (message)
	{	
	case WM_COMMAND :
		break ;

	case WM_DESTROY:
		if (m_destroyState == dsNo)
		{
			m_destroyState = dsClose;
			delete this;
		}		
		return 0 ;
	}
	return DefWindowProc(hwnd, message, wParam, lParam) ;
}

Form::~Form()
{
	if (m_destroyState == dsNo)
	{
		m_destroyState = dsFree;
		DestroyWindow(m_handle);
	};	
	free(m_wndProcStub);
	SetWindowLong(m_handle, GWL_WNDPROC, (LONG)DefWindowProc);	
	
	if (m_isMainForm)
		PostQuitMessage(0);
}
