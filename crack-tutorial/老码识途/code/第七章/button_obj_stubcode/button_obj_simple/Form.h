#pragma once

#include <windows.h>

class Form;

class FormListener
{
public:
	virtual ~FormListener(){}
	virtual void onCreate(Form * form) = 0;
};

class Form
{
public:
	enum DestroyState{dsNo, dsFree, dsClose};
	Form(bool isMainForm, WCHAR * caption);
	virtual ~Form();
	HWND getHandle(){ return m_handle; }
private:
	HWND m_handle;
	bool m_isMainForm;
	LRESULT WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void * m_wndProcStub;
	DestroyState m_destroyState;
	static LRESULT CALLBACK initWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};
