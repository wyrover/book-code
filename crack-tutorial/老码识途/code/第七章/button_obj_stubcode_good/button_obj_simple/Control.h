#pragma once
#include <windows.h>

struct WndCreateParams{
    DWORD style;
	char * className;
	WNDCLASSA wndClass;    
};

class Control;

class CtrlDestructor
{
public:
	CtrlDestructor(Control * ctrl){ m_ctrl = ctrl; }
	~CtrlDestructor(){};
	virtual void tryDestroyHandle();
	virtual void afterDestroy();
protected:
	Control * m_ctrl;
};

class ControlListener
{
public:
	virtual void onLeftButtonUp(Control * ctrl) = 0;
};

class Control
{
public:
	friend class CtrlDestructor;
	Control(int x, int y , int width, int height, char * caption, HWND parentHandle);
	virtual ~Control();
	int getX(){	return m_x;};
	void setX(int value);
	int getY(){	return m_y;};
	void setY(int value);
	int getWidth(){	return m_width;};
	void setWidth(int value);
	int getHeight(){	return m_x;};
	void setHeight(int value);
	virtual void show();
	void hide();
	HWND getHandle(){
		needHandle();
		return m_handle;
	}
	void setListener(ControlListener * listener){
		if (m_listener != NULL)
		{
			delete m_listener;
		}
		m_listener = listener;
	}

	void setCaption(char * caption);

	char * getCaption();
protected:
	virtual void createParams(WndCreateParams *param);
	virtual LRESULT WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	CtrlDestructor * m_ctrlDtor;
private:	
	ControlListener * m_listener;
	char * m_caption;
	bool m_visible;
	int m_x;
	int m_y;
	int m_width;
	int m_height;
	HWND m_handle;
	HWND m_parentHandle;
	void * m_wndProcStub;
	WNDPROC m_oldWndProc;
	LRESULT WndWrapper(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void changePosition();
	void needHandle();
	static LRESULT CALLBACK initWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};
