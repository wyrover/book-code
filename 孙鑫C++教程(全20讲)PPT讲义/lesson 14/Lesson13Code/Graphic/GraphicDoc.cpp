// GraphicDoc.cpp : implementation of the CGraphicDoc class
//

#include "stdafx.h"
#include "Graphic.h"

#include "GraphicDoc.h"
#include "GraphicView.h"
#include "Graph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphicDoc

IMPLEMENT_DYNCREATE(CGraphicDoc, CDocument)

BEGIN_MESSAGE_MAP(CGraphicDoc, CDocument)
	//{{AFX_MSG_MAP(CGraphicDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphicDoc construction/destruction

CGraphicDoc::CGraphicDoc()
{
	// TODO: add one-time construction code here

}

CGraphicDoc::~CGraphicDoc()
{
}

BOOL CGraphicDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
//	SetTitle("http://www.sunxin.org");
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGraphicDoc serialization

void CGraphicDoc::Serialize(CArchive& ar)
{
	POSITION pos=GetFirstViewPosition();
	CGraphicView *pView=(CGraphicView*)GetNextView(pos);
	if (ar.IsStoring())
	{
		// TODO: add storing code here
/*		int i=5;
		char ch='b';
		float f=1.2f;
		CString str("http://www.sunxin.org");
		ar<<i<<ch<<f<<str;*/
/*		int nCount=pView->m_obArray.GetSize();
		ar<<nCount;
		for(int i=0;i<nCount;i++)
		{
			ar<<pView->m_obArray.GetAt(i);
		}*/
	}
	else
	{
		// TODO: add loading code here
		/*int i;
		char ch;
		float f;
		CString str;
		CString strResult;
		ar>>i>>ch>>f>>str;
		strResult.Format("%d,%c,%f,%s",i,ch,f,str);
		AfxMessageBox(strResult);*/
/*		int nCount;
		ar>>nCount;
		CGraph *pGraph;
		for(int i=0;i<nCount;i++)
		{
			ar>>pGraph;
			pView->m_obArray.Add(pGraph);
		}*/
	}
//	pView->m_obArray.Serialize(ar);
	m_obArray.Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CGraphicDoc diagnostics

#ifdef _DEBUG
void CGraphicDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGraphicDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraphicDoc commands

void CGraphicDoc::DeleteContents() 
{
	// TODO: Add your specialized code here and/or call the base class
	int nCount;
	nCount=m_obArray.GetSize();
	/*for(int i=0;i<nCount;i++)
	{
		delete m_obArray.GetAt(i);
		//m_obArray.RemoveAt(i);
	}
	m_obArray.RemoveAll();*/
	while(nCount--)
	{
		delete m_obArray.GetAt(nCount);
		m_obArray.RemoveAt(nCount);
	}
	CDocument::DeleteContents();
}
