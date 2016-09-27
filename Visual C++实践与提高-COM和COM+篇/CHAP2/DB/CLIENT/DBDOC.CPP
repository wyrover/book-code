// DBDoc.cpp : implementation of the CDBDoc class
//

#include "stdafx.h"
#include "DB.h"

#include "DBDoc.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDBDoc

IMPLEMENT_DYNCREATE(CDBDoc, CDocument)

BEGIN_MESSAGE_MAP(CDBDoc, CDocument)
    //{{AFX_MSG_MAP(CDBDoc)
    ON_COMMAND(IDC_CREATE_TABLE, OnCreateTable)
    ON_COMMAND(IDC_READ_TABLE, OnReadTable)
    ON_COMMAND(IDC_WRITE_TABLE, OnWriteTable)
    ON_UPDATE_COMMAND_UI(IDC_READ_TABLE, OnUpdateReadTable)
    ON_UPDATE_COMMAND_UI(IDC_WRITE_TABLE, OnUpdateWriteTable)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBDoc construction/destruction

CDBDoc::CDBDoc()
{
    m_pDB = NULL;
}

CDBDoc::~CDBDoc()
{
    if (m_pDB) {
        delete m_pDB; //释放对象
        m_pDB = NULL;
    }
}

BOOL CDBDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    //新建数据库对象
    m_pDB = new CDB;
    // 初始化数据成员变量
    m_csData = "No data yet!";
    m_nCount = 0;
    m_nTable = -1;
    return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDBDoc serialization

void CDBDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring()) {
        // TODO: add storing code here
    } else {
        // TODO: add loading code here
    }
}

/////////////////////////////////////////////////////////////////////////////
// CDBDoc diagnostics

#ifdef _DEBUG
void CDBDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CDBDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDBDoc commands

void CDBDoc::OnCreateTable()
{
    m_pDB->Create(m_nTable, _T("Testing"));
    m_nCount = 0; // set number of writes to 0
}

void CDBDoc::OnReadTable()
{
    m_pDB->Read(m_nTable, 0, m_csData.GetBuffer(80));
    m_csData.ReleaseBuffer();
    UpdateAllViews(NULL);
}

void CDBDoc::OnWriteTable()
{
    m_nCount++;
    CString csText;
    csText.Format(_T("Test data #%d in table %d, row 0!"), m_nCount, (int) m_nTable);
    m_pDB->Write(m_nTable, 0, csText);
}

void CDBDoc::OnUpdateReadTable(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_nCount > 0);
}

void CDBDoc::OnUpdateWriteTable(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_nTable >= 0);
}
