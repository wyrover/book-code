// PhoneInfo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../PhoneInfoUILayer/PhoneInfoUI.h"
#include "../PhoneInfoLogicLayer/PhoneInfo.h"

void main(int argC, char ** args)
{
	initSys(args[0]);
	while(handleInput()); 
}

