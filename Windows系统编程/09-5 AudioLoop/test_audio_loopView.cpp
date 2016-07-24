// test_audio_loopView.cpp : implementation of the CTest_audio_loopView class
//

#include "stdafx.h"
#include "test_audio_loop.h"

#include "test_audio_loopDoc.h"
#include "test_audio_loopView.h"
#include "G723_cod.h" 
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTest_audio_loopView

IMPLEMENT_DYNCREATE(CTest_audio_loopView, CFormView)

BEGIN_MESSAGE_MAP(CTest_audio_loopView, CFormView)
	//{{AFX_MSG_MAP(CTest_audio_loopView)
	//}}AFX_MSG_MAP
	//Messages for audio input/output driver

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTest_audio_loopView construction/destruction

CTest_audio_loopView::CTest_audio_loopView()
	: CFormView(CTest_audio_loopView::IDD)
{
	//{{AFX_DATA_INIT(CTest_audio_loopView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here

	//allocate the pInfo
}

CTest_audio_loopView::~CTest_audio_loopView()
{
		//close the audio output driver
}

void CTest_audio_loopView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTest_audio_loopView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CTest_audio_loopView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTest_audio_loopView diagnostics

#ifdef _DEBUG
void CTest_audio_loopView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTest_audio_loopView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CTest_audio_loopDoc* CTest_audio_loopView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTest_audio_loopDoc)));
	return (CTest_audio_loopDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTest_audio_loopView message handlers



