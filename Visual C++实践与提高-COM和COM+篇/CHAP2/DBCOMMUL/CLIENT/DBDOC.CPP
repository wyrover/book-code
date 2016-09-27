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
        m_pDB->Release();
        m_pDB = NULL;
    }
}

BOOL CDBDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    //新建数据库对象
    //m_pDB=new CDB;
    // create a database object through the exported function & class factory object
    IClassFactory *pDBFactory = NULL;
    HRESULT hRes;
    hRes = CoGetClassObject(CLSID_DBSAMPLE, CLSCTX_SERVER, NULL, IID_IClassFactory, (void**) &pDBFactory);

    if (FAILED(hRes)) {
        CString csError;
        csError.Format(_T("Error %x obtaining class factory for DB Object!"), hRes);
        AfxMessageBox(csError);
        return FALSE;
    }

    hRes = pDBFactory->CreateInstance(NULL, IID_IUnknown, (void**) &m_pDB);

    if (FAILED(hRes)) {
        CString csError;
        csError.Format(_T("Error %x creating DB Object!"), hRes);
        AfxMessageBox(csError);
        return FALSE;
    }

    pDBFactory->Release(); // do not need the factory anymore
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
    IDBManage* pManage = NULL;

    if (FAILED(m_pDB->QueryInterface(IID_IDBManage, (void**) &pManage))) {
        AfxMessageBox(_T("Error in QueryInterface for IDBManage!"));
        return;
    }

    pManage->Create(m_nTable, L"Testing");
    m_nCount = 0; // set number of writes to 0
    pManage->Release();
}

void CDBDoc::OnReadTable()
{
    IDBAccess* pAccess = NULL;

    if (FAILED(m_pDB->QueryInterface(IID_IDBAccess, (void**) &pAccess))) {
        AfxMessageBox(_T("Error in QueryInterface for IDBAccess!"));
        return;
    }

#ifdef UNICODE
    pAccess->Read(m_nTable, 0, m_csData.GetBuffer(80));
#else
    WCHAR szuData[80];
    pAccess->Read(m_nTable, 0, szuData);
    WideCharToMultiByte(CP_ACP, 0, szuData, -1, m_csData.GetBuffer(80), 80, NULL, NULL);
#endif
    m_csData.ReleaseBuffer();
    UpdateAllViews(NULL);
    pAccess->Release();
}

void CDBDoc::OnWriteTable()
{
    IDBAccess* pAccess = NULL;

    if (FAILED(m_pDB->QueryInterface(IID_IDBAccess, (void**) &pAccess))) {
        AfxMessageBox(_T("Error in QueryInterface for IDBAccess!"));
        return;
    }

    m_nCount++;
    CString csText;
    csText.Format(_T("Test data #%d in table %d, row 0!"), m_nCount, (int) m_nTable);
#ifdef UNICODE
    pAccess->Write(m_nTable, 0, csText);
#else
    WCHAR szuText[80]; // special treatment for ASCII client
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, csText, -1, szuText, sizeof(szuText));
    pAccess->Write(m_nTable, 0, szuText);
#endif
    pAccess->Release();
}

void CDBDoc::OnUpdateReadTable(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_nCount > 0);
}

void CDBDoc::OnUpdateWriteTable(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_nTable >= 0);
}
