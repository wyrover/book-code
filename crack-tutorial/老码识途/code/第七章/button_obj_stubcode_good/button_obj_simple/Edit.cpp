#include "Edit.h"
#include "GlobalResource.h"

void Edit::createParams(WndCreateParams *param)
{
	GetClassInfoA(applicationHandle, "EDIT", &(param->wndClass));
	param->className = "TEdit";
	Control::createParams(param);
	param->style = param->style | WS_BORDER;
}