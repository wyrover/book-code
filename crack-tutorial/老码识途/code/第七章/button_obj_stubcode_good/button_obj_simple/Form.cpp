#include "Form.h"
#include "GlobalResource.h"

void FormDestructor::tryDestroyHandle()
{
	if (((Form *)m_ctrl)->m_destroyState == dsNo)
	{
		((Form *)m_ctrl)->m_destroyState = dsFree;
		DestroyWindow(((Form *)m_ctrl)->getHandle());
	};
}

void FormDestructor::afterDestroy()
{
	if (((Form *)m_ctrl)->m_isMainForm)
		PostQuitMessage(0);	
}

void Form::createParams(WndCreateParams *param)
{
	param->wndClass.style         = CS_HREDRAW | CS_VREDRAW ;
	param->wndClass.lpfnWndProc   = DefWindowProcA;
	param->wndClass.cbClsExtra    = 0 ;
	param->wndClass.cbWndExtra    = 0 ;
	param->wndClass.hInstance     = applicationHandle;
	param->wndClass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
	param->wndClass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	param->wndClass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	param->wndClass.lpszMenuName  = NULL ;
	param->wndClass.lpszClassName =  "TFORM";
	param->className = (char *)param->wndClass.lpszClassName;
	param->style = WS_OVERLAPPEDWINDOW;
}

LRESULT Form::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = Control::WndProc(hwnd, message, wParam, lParam);
	switch (message)
	{	
	case WM_COMMAND :
		//could find ctrls here and trigger event.
		return 0;
		break ;

	case WM_DESTROY:
		if (m_destroyState == dsNo)
		{
			m_destroyState = dsClose;
			delete this;
		}		
		return 0 ;
	}	
	return ret;
}