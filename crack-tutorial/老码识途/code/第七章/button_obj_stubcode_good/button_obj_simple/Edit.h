#pragma once
#include <windows.h>
#include "Control.h"

class Edit: public Control
{
public:
	Edit(int x, int y , int width, int height, char * caption, HWND parentHandle) : Control(x, y, width, height, caption, parentHandle){}
protected:
	void createParams(WndCreateParams *param);
};
