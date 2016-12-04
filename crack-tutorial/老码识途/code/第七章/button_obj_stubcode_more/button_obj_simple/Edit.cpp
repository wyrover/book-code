#include "Edit.h"
#include <string.h>
#include <malloc.h>
#include "GlobalResource.h"

Edit::Edit(int x, int y, int width, int height, char * editName, HWND parentHandle)
{	m_handle = CreateWindowA("EDIT", 
		editName,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		x, y,
		width, height,
		parentHandle, 0,
		applicationHandle, NULL);
	LRESULT (__thiscall Edit::* ptrFunc)(HWND,UINT,WPARAM,LPARAM);
	ptrFunc = (&Edit::WndProc);
	void * func;
	__asm{
		mov eax, ptrFunc;
		mov func, eax;
	}
	m_wndProcStub = makeWndProcStub(this, func);
	m_oldWndProc = (WNDPROC)SetWindowLong(m_handle, GWL_WNDPROC, (LONG)m_wndProcStub);	
	m_text = NULL;
}

Edit::~Edit()
{
	if (m_text != NULL)
	{
		free(m_text);
	}
	DestroyWindow(m_handle);
	free(m_wndProcStub);
	SetWindowLong(m_handle, GWL_WNDPROC, (LONG)m_oldWndProc);	
}

void Edit::setText(char * value)
{
	SendMessageA(m_handle, WM_SETTEXT, 0, (LPARAM)value);
}

char * Edit::getText()
{
	int len = SendMessageA(m_handle, WM_GETTEXTLENGTH, 0, 0);
	if (m_text != NULL)
	{
		free(m_text);
	}
	m_text = (char *)malloc(len + 1);
	SendMessageA(m_handle, WM_GETTEXT, len + 1, (LPARAM)m_text);
	return m_text;
}

LRESULT Edit::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{	

	LRESULT result = CallWindowProcW(m_oldWndProc, hwnd, message, wParam, lParam);
	//if need add listner, call here	
	return result;	
}