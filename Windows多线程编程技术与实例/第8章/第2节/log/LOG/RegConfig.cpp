// RegConfig.cpp: implementation of the CRegConfig class.
// Written by Alexander Hritonenkov on march 19, 2003
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "RegConfig.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegConfig::CRegConfig(HKEY hKey, LPCTSTR strKey)
{
	SetRegKey(hKey, strKey);
}

CRegConfig::CRegConfig()
{
	m_hKey=NULL;
	m_strKey="";
}

CRegConfig::~CRegConfig()
{
}

void CRegConfig::SetRegKey(HKEY hKey, LPCTSTR strKey)
{
	m_hKey=hKey;
	m_strKey=strKey;
}

void CRegConfig::Save()
{
	if(m_rKey.Create(m_hKey, m_strKey)==ERROR_SUCCESS)
	{
		TransferAllData(TRUE);
		m_rKey.Close();
	}
}

void CRegConfig::Load()
{
	if(m_rKey.Create(m_hKey, m_strKey)==ERROR_SUCCESS)
	{
		TransferAllData(FALSE);
		m_rKey.Close();
	}
}

void CRegConfig::Transfer(BOOL bSave, LPCTSTR strValueName, DWORD &dwValue)
{
	if(bSave)
		m_rKey.SetValue(dwValue, strValueName);
	else
		m_rKey.QueryValue(dwValue, strValueName);
}

void CRegConfig::Transfer(BOOL bSave, LPCTSTR strValueName, CString &strValue)
{
	if(bSave)
		m_rKey.SetValue(strValue, strValueName);
	else
	{
		DWORD dwLen;
		LPTSTR lpStr;
		if(m_rKey.QueryValue(NULL, strValueName, &dwLen)==ERROR_SUCCESS)
		{
			lpStr=strValue.GetBuffer(dwLen+1);
			m_rKey.QueryValue(lpStr, strValueName, &dwLen);
			strValue.ReleaseBuffer(dwLen);
		}
	}
}

void CRegConfig::Transfer(BOOL bSave, LPCTSTR strValueName, CByteArray &bValue)
{
	DWORD dwLen, dwType;
	LPBYTE lpData;
	if(bSave)
	{
		dwLen=bValue.GetSize();
		lpData=bValue.GetData();
		RegSetValueEx(m_rKey.m_hKey, strValueName, 0, REG_BINARY, lpData, dwLen);
	}
	else
	{
		dwType=REG_BINARY;
		if(RegQueryValueEx(m_rKey.m_hKey, strValueName, NULL, &dwType, NULL, &dwLen)==ERROR_SUCCESS)
		{
			bValue.RemoveAll();
			bValue.SetSize(dwLen);
			lpData=bValue.GetData();
			RegQueryValueEx(m_rKey.m_hKey, strValueName, NULL, &dwType, lpData, &dwLen);
		}
	}
}
