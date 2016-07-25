// DBConnetParamRegConfig.h: interface for the CDBConnetParamRegConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBCONNETPARAMREGCONFIG_H__2B64E756_CE1C_456C_8A33_BD1783EECEBB__INCLUDED_)
#define AFX_DBCONNETPARAMREGCONFIG_H__2B64E756_CE1C_456C_8A33_BD1783EECEBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RegConfig.h"

class CDBConnetParamRegConfig : public CRegConfig  
{
public:
	CDBConnetParamRegConfig();
	virtual ~CDBConnetParamRegConfig();
	// data members
	CString srvName;
	CString DBName;
	CString loginNm;
	CString pwd;
	
	
protected:
	virtual void TransferAllData(BOOL bSave);	// overrided pure virtual function for perform saving and loading data
	
	
};

#endif // !defined(AFX_DBCONNETPARAMREGCONFIG_H__2B64E756_CE1C_456C_8A33_BD1783EECEBB__INCLUDED_)
