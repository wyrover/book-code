// CarusoeServerFile.cpp: implementation of the CCarusoeServerFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CarusoeServerFile.h"

#ifdef _DEBUG
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
    #define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCarusoeServerFile::CCarusoeServerFile()
{
    m_pos = NULL;
}

CCarusoeServerFile::~CCarusoeServerFile()
{
}

void CCarusoeServerFile::Initialize(LPCTSTR serverFile)
{
    try {
        CString tableName;
        CFile file(serverFile, CFile::modeRead | CFile::shareDenyNone);
        LPBYTE pByte = new BYTE[file.GetLength() + 1];
        memset(pByte, NULL, file.GetLength() + 1);
        file.Read(pByte, file.GetLength());
        LPBYTE pStartPos = pByte;
        LPBYTE pCurPos = pByte;

        while (TRUE) {
            if (*pCurPos == '\r' || *pCurPos == '\n' || *pCurPos == '\0') {
                LPTSTR pTableName = tableName.GetBuffer(pCurPos - pStartPos);
                _tcsncpy(pTableName, (char*)pStartPos, pCurPos - pStartPos);
                tableName.ReleaseBuffer(pCurPos - pStartPos);
                m_serverTableNames.Add(tableName);

                if (*pCurPos == '\0')
                    break;

                while (*pCurPos == '\r' || *pCurPos == '\n')
                    pCurPos++;

                pStartPos = pCurPos;
                continue;
            }

            pCurPos ++;
        }

        delete [] pByte;
    } catch (CException& e) {
        e.ReportError();
    }
}

long CCarusoeServerFile::GetTableCount()
{
    return m_serverTableNames.GetSize();
}

BOOL CCarusoeServerFile::GetTableAt(long ndx, CComBSTR& bstrTableName)
{
    if (ndx >= m_serverTableNames.GetSize())
        return FALSE;

    bstrTableName = m_serverTableNames.GetAt(ndx);
    return TRUE;
}
