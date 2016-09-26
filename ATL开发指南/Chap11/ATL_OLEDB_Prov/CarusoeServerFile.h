// CarusoeServerFile.h: interface for the CCarusoeServerFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CARUSOESERVERFILE_H__56A849C1_7A9D_11D3_9D83_EA0F5EEF7333__INCLUDED_)
#define AFX_CARUSOESERVERFILE_H__56A849C1_7A9D_11D3_9D83_EA0F5EEF7333__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

class CCarusoeServerFile
{
public:
    CCarusoeServerFile();
    virtual ~CCarusoeServerFile();

    void Initialize(LPCTSTR serverFile);

    BOOL GetTableAt(long ndx, CComBSTR& bstrTableName);
    long GetTableCount();
protected:
    POSITION m_pos;
    CStringArray m_serverTableNames;
    CString m_serverFileName;
};

#endif // !defined(AFX_CARUSOESERVERFILE_H__56A849C1_7A9D_11D3_9D83_EA0F5EEF7333__INCLUDED_)
