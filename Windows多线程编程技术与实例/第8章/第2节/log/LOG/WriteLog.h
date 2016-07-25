// WriteLog.h: interface for the CWriteLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WRITELOG_H__F367A6B3_C553_451D_930E_5BE9105EB85C__INCLUDED_)
#define AFX_WRITELOG_H__F367A6B3_C553_451D_930E_5BE9105EB85C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include <string>
#include <map>
using namespace std;


#include "ReadLog.h"

class CWriteLog  
{
public:
	void writeLog();
	void initWriteLog();
	CWriteLog();
	virtual ~CWriteLog();
protected:


	_RecordsetPtr m_pRecordsetSelect;
	
	_ConnectionPtr m_pConnection;

	
	CString sql;
	BOOL  exist;
	
	void setConnect();


	
	BOOL AppendChunk(FieldPtr pField, LPVOID lpData, UINT nBytes);
	

	
	
	void lastDeleteOutOfTimeFile(CString& strDir,CString& fileName);
	BOOL compareTwoLogFileTime(CString& filename1,CString& filename2);
	
private:

	UINT m_CountAfterDel;
	CString m_strTimeStamp;
};

#endif // !defined(AFX_WRITELOG_H__F367A6B3_C553_451D_930E_5BE9105EB85C__INCLUDED_)
