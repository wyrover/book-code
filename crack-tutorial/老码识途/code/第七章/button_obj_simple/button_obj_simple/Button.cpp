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
	addWindowHandle(m_handle, this);
	m_oldWndProc = (WNDPROC)SetWindowLong(m_handle, GWL_WNDPROC, (LONG)WndProc);
	m_caption = NULL;
	setCaption(caption);	
	m_listener = NULL;
}

LRESULT CALLBACK Button::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	Button * self = (Button *)getObject(hwnd);
	if (self == NULL)
	{
		return 0;
	}
	LRESULT result = CallWindowProcA(self->m_oldWndProc, hwnd, message, wParam, lParam);
	if (self->m_listener != NULL)
	{
		if (message == WM_LBUTTONUP)
		{
			self->m_listener->onLeftButtonUp(self);
		}		
	}
	return result;	
}

Button::~Button(void)
{
	delWindowHandle(m_handle);	
	if (m_listener != NULL)
	{
		delete m_listener;
	}
	if (m_caption != NULL)
	{
		free(m_caption);
	}
	DestroyWindow(m_handle);
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
