// MultiCastChatView.cpp : implementation of the CMultiCastChatView class
//

#include "stdafx.h"
#include "MultiCastChat.h"

#include "MultiCastChatDoc.h"
#include "CntrItem.h"
#include "MultiCastChatView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiCastChatView

IMPLEMENT_DYNCREATE(CMultiCastChatView, CRichEditView)

BEGIN_MESSAGE_MAP(CMultiCastChatView, CRichEditView)
	//{{AFX_MSG_MAP(CMultiCastChatView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	ON_WM_DESTROY()
	ON_MESSAGE(WM_MULTIPOINTEVENT, OnReceive)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CRichEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiCastChatView construction/destruction

CMultiCastChatView::CMultiCastChatView()
{

}

CMultiCastChatView::~CMultiCastChatView()
{
}

BOOL CMultiCastChatView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CRichEditView::PreCreateWindow(cs);
}

void CMultiCastChatView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();

	cfm.cbSize=sizeof(cfm);
	cfm.bCharSet=GB2312_CHARSET;
	cfm.crTextColor=RGB(0,0,0);
	cfm.dwMask=CFM_CHARSET | CFM_COLOR ;
	GetRichEditCtrl().SetDefaultCharFormat(cfm);

	// Set the printing margins (720 twips = 1/2 inch).
	SetMargins(CRect(720, 720, 720, 720));
}

/////////////////////////////////////////////////////////////////////////////
// CMultiCastChatView printing

BOOL CMultiCastChatView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}


void CMultiCastChatView::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used.
   CRichEditView::OnDestroy();
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
}


/////////////////////////////////////////////////////////////////////////////
// CMultiCastChatView diagnostics

#ifdef _DEBUG
void CMultiCastChatView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CMultiCastChatView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CMultiCastChatDoc* CMultiCastChatView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMultiCastChatDoc)));
	return (CMultiCastChatDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMultiCastChatView message handlers

LONG CMultiCastChatView::OnReceive(WPARAM wParam, LPARAM lParam)
{
	SOCKET EventSocket = (SOCKET)wParam;
	if(WSAGETSELECTERROR(lParam) != 0)
	{
		int nError = WSAGetLastError();
		TRACE("Error in receving data packet!\n");
		return 0L;
	}
	switch(WSAGETSELECTEVENT(lParam))
	{
	case FD_WRITE:
		TRACE("Now can sending data now!\n");
		break;
	case FD_READ:
		GetDocument()->Receive();
		break;
	case FD_QOS:
		break;
	case FD_GROUP_QOS:
		break;
	default:
		break;
	}
	return 0L;
}

void CMultiCastChatView::TextOut(LPCTSTR lpszMessage, COLORREF clr)
{
	cfm.cbSize=sizeof(cfm);
	cfm.crTextColor=clr;
	cfm.dwMask=CFM_COLOR;
	CString strTemp = lpszMessage;
	
	int len = GetWindowTextLength();
	GetRichEditCtrl().SetSel(len,len);
	
	GetRichEditCtrl().SetSelectionCharFormat(cfm);
	GetRichEditCtrl().ReplaceSel(strTemp);
}
