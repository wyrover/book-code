#pragma once

class  CLuaCore : public LuaStateOwner
{
public:
	CLuaCore();
	CRITICAL_SECTION	m_csLock;
	~CLuaCore();
	void Lock();
	void UnLock();

};
extern CLuaCore	gLuaCore;

void RegisterLuaApis();
int DoFile(char* plpFile);