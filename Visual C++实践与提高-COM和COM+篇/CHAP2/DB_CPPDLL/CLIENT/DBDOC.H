// DBDoc.h : interface of the CDBDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBDOC_H__84FCEE82_5F09_11D4_A23D_02608CACEDBB__INCLUDED_)
#define AFX_DBDOC_H__84FCEE82_5F09_11D4_A23D_02608CACEDBB__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#include "..\interface\DBsrv.h"

class CDBDoc : public CDocument
{
protected: // create from serialization only
    CDBDoc();
    DECLARE_DYNCREATE(CDBDoc)

// Attributes
public:
    CDB *m_pDB; // pointer to database object
    CString m_csData; // last data read from database
    int m_nCount;           // number of writes to database
    short m_nTable;     // number of last table created
// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDBDoc)
public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CDBDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    //{{AFX_MSG(CDBDoc)
    afx_msg void OnCreateTable();
    afx_msg void OnReadTable();
    afx_msg void OnWriteTable();
    afx_msg void OnUpdateReadTable(CCmdUI* pCmdUI);
    afx_msg void OnUpdateWriteTable(CCmdUI* pCmdUI);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBDOC_H__84FCEE82_5F09_11D4_A23D_02608CACEDBB__INCLUDED_)
