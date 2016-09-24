#include "StdAfx.h"
#include "LuaCore.h"
#include "../../Hooklib/hooklib.h"
#include "WorkCore.h"
#include <vector>
#include <string>
using namespace std;

vector<string>	gFuncName;
CLuaCore		gLuaCore;

BOOL Lua_SetBreakPoint(void* plpFunc,const char* plpCallBackName)
{
	return FALSE;
}

void Lua_Printf(const char* plpPrint)
{
	MessageBox(0,plpPrint,"",MB_OK);
}

CLuaCore::CLuaCore()
{
	InitializeCriticalSection(&m_csLock);
}

CLuaCore::~CLuaCore()
{
	DeleteCriticalSection(&m_csLock);
}

void CLuaCore::Lock()
{
	EnterCriticalSection(&m_csLock);
}

void CLuaCore::UnLock()
{
	LeaveCriticalSection(&m_csLock);
}

void RegisterLuaApis()
{
	gLuaCore.Lock();
	gLuaCore->GetGlobals().RegisterDirect("printf",Lua_Printf);
	gLuaCore->GetGlobals().RegisterDirect("SetBreakPoint",Lua_SetBreakPoint);
	gLuaCore.UnLock();
}

int DoFile( char* plpFile )
{
	gLuaCore.Lock();
	int pRet = gLuaCore->DoFile(plpFile);
	gLuaCore.UnLock();
	return pRet;
}