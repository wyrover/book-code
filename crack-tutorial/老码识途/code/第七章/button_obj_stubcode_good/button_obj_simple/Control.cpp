#include "Control.h"
#include <string.h>
#include <malloc.h>
#include "GlobalResource.h"

void CtrlDestructor::tryDestroyHandle()
{
	DestroyWindow(m_ctrl->m_handle);
}

void CtrlDestructor::afterDestroy()
{

}

Control::Control(int x, int y , int width, int height, char * caption, HWND parentHandle)
{	
	m_ctrlDtor = new CtrlDestructor(this);
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
	m_caption = NULL;
	if (caption != NULL)
	{
		int len = strlen(caption);
		if (len > 0)
		{
			m_caption = (char *)malloc(strlen(caption) + 1);
			strcpy(m_caption, caption);
		}
	}	
	m_parentHandle = parentHandle;
	m_visible = false;
	m_handle = 0;
	m_listener = NULL;
}

Control::~Control()
{
	if (m_caption != NULL)
	{
		free(m_caption);
	}
	if (m_handle != 0)
	{
		m_ctrlDtor->tryDestroyHandle();		
		free(m_wndProcStub);
		SetWindowLong(m_handle, GWL_WNDPROC, (LONG)m_oldWndProc);	
	}
	if (m_listener != NULL)
	{
		delete m_listener;
	}
	m_ctrlDtor->afterDestroy();
	delete m_ctrlDtor;
}

void Control::setX(int value)
{
	if (m_x != value)
	{
		m_x = value;
		changePosition();
	}
}

void Control::setY(int value)
{
	if (m_y != value)
	{
		m_y = value;
		changePosition();
	}
}

void Control::setWidth(int value)
{
	if (m_width != value)
	{
		m_width = value;
		changePosition();
	}
}

void Control::setHeight(int value)
{
	if (m_height != value)
	{
		m_height = value;
		changePosition();
	}
}

void Control::changePosition()
{
	if (!m_visible)
	{
		return;
	}
	needHandle();
	SetWindowPos(m_handle, 0, m_x, m_y, m_width, m_height, SWP_SHOWWINDOW);
}

void Control::show()
{
	if (m_visible)
	{
		return;
	}
	m_visible = true;
	changePosition();
}

void Control::hide()
{
	if (!m_visible)
	{
		return;
	}
	m_visible = false;	
	SetWindowPos(m_handle, 0, m_x, m_y, m_width, m_height, SWP_HIDEWINDOW);
}

void Control::createParams(WndCreateParams *param)
{
	param->style = WS_CHILD | WS_VISIBLE;
}

Control * gCtrlNoFinished;

LRESULT CALLBACK Control::initWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//此时，是createwindow激活wm_create进入该函数，所以createwindow未返回，因此m_handle没有设定值，为保证消息处理函数能使用成员变量m_handle所以设定
	gCtrlNoFinished->m_handle = hwnd;
	SetWindowLongA(hwnd, GWL_WNDPROC, (LONG)gCtrlNoFinished->m_wndProcStub);	 	
	void * func = gCtrlNoFinished->m_wndProcStub;
	__asm{
		push lParam;
		push wParam;
		push message;
		push hwnd;
		mov ecx, gCtrlNoFinished;
		call func;		
	}
}

LRESULT Control::WndWrapper(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return WndProc(hwnd, message, wParam, lParam);
}

LRESULT Control::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = CallWindowProcA(m_oldWndProc, hwnd, message, wParam, lParam);		
	if (m_listener != NULL)
	{
		switch (message)
		{
		case WM_LBUTTONUP: 
			m_listener->onLeftButtonUp(this);
			break;
		}				
	}	
	return result;
}

void Control::needHandle()
{
	WndCreateParams params;
	if (m_handle == 0)
	{
		LRESULT (__thiscall Control::* ptrFunc)(HWND,UINT,WPARAM,LPARAM);
		ptrFunc = (&Control::WndWrapper);
		void * func;
		__asm{
			mov eax, ptrFunc;
			mov func, eax;
		}
		m_wndProcStub = makeWndProcStub(this, func);	 
		gCtrlNoFinished = this;
		createParams(&params);
		m_oldWndProc = params.wndClass.lpfnWndProc;
		WNDCLASSA tmpClass;
		bool registered = GetClassInfoA(applicationHandle, params.className, &tmpClass);
		if (!registered || params.wndClass.lpfnWndProc != initWndProc)
		{
			if (registered)
			{
				UnregisterClassA(params.className, applicationHandle);
			}
			params.wndClass.lpfnWndProc = initWndProc;
			params.wndClass.lpszClassName = params.className;
			RegisterClassA(&params.wndClass);
		}
		m_handle = CreateWindowA(params.wndClass.lpszClassName, 
			m_caption,
			params.style,
			m_x, m_y,
			m_width, m_height,
			m_parentHandle, 0,
			applicationHandle, NULL) ;		
		/*if (m_visible == false)
		{
			SetWindowPos(m_handle, 0, m_x, m_y, m_width, m_height, SWP_HIDEWINDOW);
		}*/
	}
}

void Control::setCaption(char * caption)
{
	if (m_caption != NULL)
	{
		free(m_caption);
	}
	m_caption = (char *)malloc(strlen(caption) + 1);
	strcpy(m_caption, caption);
	SendMessageA(m_handle, WM_SETTEXT, 0, (LPARAM)caption);
}

char * Control::getCaption()
{
	int len = SendMessageA(m_handle, WM_GETTEXTLENGTH, 0, 0);
	if (m_caption != NULL)
	{
		free(m_caption);
	}
	m_caption = (char *)malloc(len + 1);
	SendMessageA(m_handle, WM_GETTEXT, len + 1, (LPARAM)m_caption);
	return m_caption;
}

