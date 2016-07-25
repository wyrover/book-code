// DBConnetParamRegConfig.cpp: implementation of the CDBConnetParamRegConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "asc1.h"
#include "DBConnetParamRegConfig.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBConnetParamRegConfig::CDBConnetParamRegConfig() : CRegConfig(HKEY_CURRENT_USER, "Software\\iXRayASC\\option\\dbconnection")
{
	// set default values for our settings
	// they are used on first start (if data is not present in registry)
	srvName = "";
	DBName = "";
	loginNm = "";
	pwd = "";
	//如果注册表中还没有此键的值,将不更新,如果有则用注册表中的值来更新该类的成员
	//	Load();	// you can add automatic load of config on construction
}

CDBConnetParamRegConfig::~CDBConnetParamRegConfig()
{
	//	Save();	// you can add automatic save of config on destruction
}

void CDBConnetParamRegConfig::TransferAllData(BOOL bSave)
{
	// add Transfer() function for each data member in your class
	Transfer(bSave, "SrvNm", srvName);
	Transfer(bSave, "DBNm", DBName);
	Transfer(bSave, "LogNm", loginNm);
	Transfer(bSave, "LogPWD", pwd);
	
}

