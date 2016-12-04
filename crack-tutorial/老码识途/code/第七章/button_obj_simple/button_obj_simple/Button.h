#pragma once

#include <windows.h>

class Button;

class ButtonListener
{
public:
	virtual ~ButtonListener(){

	}
//	virtual onClick(Button * button) = 0;
	virtual void onLeftButtonUp(Button * button) = 0;
};

class Button
{
public:
	Button(int x, int y, int width, int height, char * buttonName, HWND parentHandle);
	virtual ~Button();
	void setCaption(char * caption);
	char * getCaption(){ return m_caption; }
	void setListener(ButtonListener * listener){ m_listener = listener;}
	HWND getHandle(){ return m_handle;}
private:
	HWND m_handle;
	WNDPROC m_oldWndProc;
	char * m_caption;
	ButtonListener * m_listener;
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};
