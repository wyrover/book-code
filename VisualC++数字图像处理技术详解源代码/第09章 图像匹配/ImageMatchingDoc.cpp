// ImageMatchingDoc.cpp : implementation of the CImageMatchingDoc class
//

#include "stdafx.h"
#include "ImageMatching.h"

#include "ImageMatchingDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageMatchingDoc

IMPLEMENT_DYNCREATE(CImageMatchingDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageMatchingDoc, CDocument)
	//{{AFX_MSG_MAP(CImageMatchingDoc)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageMatchingDoc construction/destruction

CImageMatchingDoc::CImageMatchingDoc()
{
	// TODO: add one-time construction code here
    InitDocVariable();
}

CImageMatchingDoc::~CImageMatchingDoc()
{
	FreeDocVariable();
}

BOOL CImageMatchingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CImageMatchingDoc serialization

void CImageMatchingDoc::Serialize(CArchive& ar)
{
	m_pDibInit->Serialize(ar);
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CImageMatchingDoc diagnostics

#ifdef _DEBUG
void CImageMatchingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageMatchingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageMatchingDoc commands

int CImageMatchingDoc::FreeDocVariable()
{
    delete m_pDibInit;
	m_pDibInit = NULL;

	// added by tanhc in 2002-7-30, used to test some function
	delete m_pDibTest ;
	m_pDibTest = NULL ;

	//--------------------------------------------

	return 0;
}

int CImageMatchingDoc::InitDocVariable()
{
	m_pDibInit = new CDib;

	// added by tanhc in 2002-7-30, used to test some function
	m_pDibTest = new CDib;

	m_nColorIndex = 0;

	//--------------------------------------------

	return 0;
}

void CImageMatchingDoc::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	CString strSaveFileType = "Î»Í¼ÎÄ¼þ (*.bmp;*.dib)|*.bmp; *.dib|All Files (*.*)|*.*||";
	CFileDialog FileDlg(FALSE, "*.bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strSaveFileType);

	CFile fileOpen ;
	if( FileDlg.DoModal() == IDOK ) {
		if(!fileOpen.Open( FileDlg.GetPathName() , CFile::modeCreate|CFile::modeWrite )){
			AfxMessageBox("cannot create the file to save");
			return;
		}
		if( !m_pDibInit->Write( &fileOpen ) ){
			return;
		}
		fileOpen.Close();
	}
}
