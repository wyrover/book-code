#pragma once

#include <windows.h>
#include "Control.h"

class Button : public Control
{
public:
	Button(int x, int y , int width, int height, char * caption, HWND parentHandle) : Control(x, y, width, height, caption, parentHandle){}
protected:
	void createParams(WndCreateParams *param);
};
