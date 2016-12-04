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
	Form(bool isMainForm, WCHAR * caption);
	virtual ~Form();
	HWND getHandle(){ return m_handle; }
private:
	HWND m_handle;
	bool m_isMainForm;
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};
