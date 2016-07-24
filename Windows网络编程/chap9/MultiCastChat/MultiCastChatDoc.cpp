// MultiCastChatDoc.cpp : implementation of the CMultiCastChatDoc class
//

#include "stdafx.h"
#include "MultiCastChat.h"

#include "MultiCastChatDoc.h"
#include "CntrItem.h"
#include "multicastchatview.h"
#include "mainfrm.h"
#include "adddialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiCastChatDoc

IMPLEMENT_DYNCREATE(CMultiCastChatDoc, CRichEditDoc)

BEGIN_MESSAGE_MAP(CMultiCastChatDoc, CRichEditDoc)
	//{{AFX_MSG_MAP(CMultiCastChatDoc)
	ON_COMMAND(IDC_TEST, OnTest)
	ON_COMMAND(IDC_SEND, OnSend)
	//}}AFX_MSG_MAP
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, CRichEditDoc::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, CRichEditDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, CRichEditDoc::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiCastChatDoc construction/destruction

CMultiCastChatDoc::CMultiCastChatDoc()
{
	str = new char[1024];
	CMultiCast::Initilize();
}

CMultiCastChatDoc::~CMultiCastChatDoc()
{
	CMultiCast::Unintilize();
	delete [] str;
}

BOOL CMultiCastChatDoc::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;

	return TRUE;
}

CRichEditCntrItem* CMultiCastChatDoc::CreateClientItem(REOBJECT* preo) const
{
	return new CMultiCastChatCntrItem(preo, (CMultiCastChatDoc*) this);
}



/////////////////////////////////////////////////////////////////////////////
// CMultiCastChatDoc serialization

void CMultiCastChatDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}

	// Calling the base class CRichEditDoc enables serialization
	//  of the container document's COleClientItem objects.
	// TODO: set CRichEditDoc::m_bRTF = FALSE if you are serializing as text
	CRichEditDoc::Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CMultiCastChatDoc diagnostics

#ifdef _DEBUG
void CMultiCastChatDoc::AssertValid() const
{
	CRichEditDoc::AssertValid();
}

void CMultiCastChatDoc::Dump(CDumpContext& dc) const
{
	CRichEditDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMultiCastChatDoc commands

void CMultiCastChatDoc::OnTest() 
{
	CAddDialog dlg;
	if(dlg.DoModal() == IDOK){
		m_socket.Create(dlg.m_strAddr, dlg.m_nPort, GetView()->m_hWnd);
		CMultiCastChatView* pView = (CMultiCastChatView*)GetView();
		CString t("You hav Join the Group: ");
		t += dlg.m_strAddr;
		CString tmp;
		tmp.Format("  at port: %d\n", dlg.m_nPort);
		t += tmp;
		pView->TextOut(t, RGB(0,100,255));
	}
}

void CMultiCastChatDoc::Receive()
{
	memset(str, 0, 1024);
	m_socket.Receive(str, 1024);
	CString s;
	s += str;
	s += "\n";
	CMultiCastChatView* pView = (CMultiCastChatView*)GetView();
	pView->TextOut(s, RGB(0,0,0));
}

void CMultiCastChatDoc::OnSend() 
{
	if(!m_socket.IsConnected()){
		CMultiCastChatView* pView = (CMultiCastChatView*)GetView();
		pView->TextOut("You havn't Join any Group!\n", RGB(255,100,100));
		return;
	}

	CMainFrame* pFrame = (CMainFrame*)GetView()->GetParent();
	CWnd* pText = pFrame->m_wndSend.GetDlgItem(IDC_TEXT);
	CString tmp;
	pText->GetWindowText(tmp);
	if(!tmp.Compare(""))
		return;
	pText->SetWindowText(_T(""));

	char* str = tmp.GetBuffer(0);
	m_socket.Send(str, strlen(str));
//	TRACE("Here send a packet!\n");
}
