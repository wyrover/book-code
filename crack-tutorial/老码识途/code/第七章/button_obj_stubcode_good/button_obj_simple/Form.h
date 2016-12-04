#pragma once

#include <windows.h>
#include "Control.h"

enum DestroyState{dsNo, dsFree, dsClose};

class Form;
class FormDestructor : public CtrlDestructor
{
public:	
	FormDestructor(Control * ctrl) : CtrlDestructor(ctrl){}
	void tryDestroyHandle();
	void afterDestroy();
};

class Form : public Control
{
public:
	friend class FormDestructor;	
	Form(bool isMainForm, char * caption) : Control(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, caption, 0){
		delete m_ctrlDtor;
		m_ctrlDtor = new FormDestructor(this);
		m_isMainForm = isMainForm;
		m_destroyState = dsNo;	
	}	
	virtual void show(){
		ShowWindow(getHandle(), SW_SHOW);
		UpdateWindow(getHandle());
	}
protected:
	void createParams(WndCreateParams *param);
	virtual LRESULT WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	bool m_isMainForm;	
	DestroyState m_destroyState;	
};
