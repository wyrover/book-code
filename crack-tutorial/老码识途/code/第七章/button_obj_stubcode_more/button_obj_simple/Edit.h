#pragma once
#include <windows.h>

class Edit
{
public:
	Edit(int x, int y, int width, int height, char * editName, HWND parentHandle);
	virtual ~Edit();
	void setText(char * value);
	char* getText();
private:
	WNDPROC m_oldWndProc;
	HWND m_handle;
	LRESULT WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void * m_wndProcStub;
	char * m_text;
};
