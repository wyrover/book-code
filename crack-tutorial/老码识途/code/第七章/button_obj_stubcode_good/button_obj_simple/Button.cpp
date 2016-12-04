#include "Button.h"
#include "GlobalResource.h"

void Button::createParams(WndCreateParams *param)
{
	GetClassInfoA(applicationHandle, "BUTTON", &(param->wndClass));
	param->className = "TButton";
	Control::createParams(param);
	param->style = param->style | BS_PUSHBUTTON;
}