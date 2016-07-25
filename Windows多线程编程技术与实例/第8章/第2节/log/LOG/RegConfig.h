// RegConfig.h: interface for the CRegConfig class.
// Written by Alexander Hritonenkov on march 19, 2003
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGCONFIG_H__7E051819_5CAA_4E49_99F2_0236B42086A4__INCLUDED_)
#define AFX_REGCONFIG_H__7E051819_5CAA_4E49_99F2_0236B42086A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "atlbase.h"

class CRegConfig  
{
public:
	void SetRegKey(HKEY hKey, LPCTSTR strKey);	// Sets registry key to keep settings in
	void Load();	// Loads settings from registry
	void Save();	// Saves settings to registry
	CRegConfig(HKEY hKey, LPCTSTR strKey);	// Constructor that sets registry key to keep settings in
	CRegConfig();	// Default constructor
	virtual ~CRegConfig();	// Destructor (empty)
	
protected:
	HKEY m_hKey;	// Parent key descriptor
	CString m_strKey;	// Our key full name
	CRegKey m_rKey;
	virtual void TransferAllData(BOOL bSave)=0;	// Data transfering function
	void Transfer(BOOL bSave, LPCTSTR strValueName, CString &strValue);	// Function to load/save string data
	void Transfer(BOOL bSave, LPCTSTR strValueName, DWORD &dwValue);	// Function to load/save DWORD data
	void Transfer(BOOL bSave, LPCTSTR strValueName, CByteArray &bValue);	// Function to load/save binary data
};

#endif // !defined(AFX_REGCONFIG_H__7E051819_5CAA_4E49_99F2_0236B42086A4__INCLUDED_)
