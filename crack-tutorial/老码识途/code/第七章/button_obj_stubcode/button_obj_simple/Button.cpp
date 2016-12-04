#include "Button.h"
#include "GlobalResource.h"
#include <string.h>
#include <malloc.h>

Button::Button(int x, int y, int width, int height, char * caption, HWND parentHandle)
{
	m_handle = CreateWindowA("BUTTON", 
		caption,
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		x, y,
		width, height,
		parentHandle, 0,
		applicationHandle, NULL) ;
	LRESULT (__thiscall Button::* ptrFunc)(HWND,UINT,WPARAM,LPARAM);
	ptrFunc = (&Button::WndProc);
	void * func;
	__asm{
		mov eax, ptrFunc;
		mov func, eax;
	}
	m_wndProcStub = makeWndProcStub(this, func);
	m_oldWndProc = (WNDPROC)SetWindowLong(m_handle, GWL_WNDPROC, (LONG)m_wndProcStub);
	m_caption = NULL;
	setCaption(caption);	
	m_listener = NULL;
}

LRESULT Button::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	
	LRESULT result = CallWindowProcA(m_oldWndProc, hwnd, message, wParam, lParam);
	if (m_listener != NULL)
	{
		if (message == WM_LBUTTONUP)
		{
			m_listener->onLeftButtonUp(this);
		}		
	}
	return result;	
}

Button::~Button(void)
{
	if (m_listener != NULL)
	{
		delete m_listener;
	}
	if (m_caption != NULL)
	{
		free(m_caption);
	}
	DestroyWindow(m_handle);
	free(m_wndProcStub);
	SetWindowLong(m_handle, GWL_WNDPROC, (LONG)m_oldWndProc);	
}

void Button::setCaption(char * caption)
{
	if (m_caption != NULL)
	{
		free(m_caption);
	}
	m_caption = (char *)malloc(strlen(caption) + 1);
	strcpy(m_caption, caption);
	SendMessageA(m_handle, WM_SETTEXT, 0, (LPARAM)caption);
}
