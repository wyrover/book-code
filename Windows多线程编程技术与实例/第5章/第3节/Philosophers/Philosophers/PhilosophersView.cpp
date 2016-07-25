// PhilosophersView.cpp : implementation of the CPhilosophersView class
//

#include "stdafx.h"
#include "Philosophers.h"

#include "PhilosophersDoc.h"
#include "PhilosophersView.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL chopSticks[PHIL_NUM] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};
BOOL Finished[PHIL_NUM] ={TRUE, TRUE, TRUE, TRUE, TRUE, TRUE};
CMutex *m_pMutex = NULL;
CEvent *m_pEvent = NULL;

UINT DineOnce(LPVOID pParam);
UINT DineMany(LPVOID pParam);
/////////////////////////////////////////////////////////////////////////////
// CPhilosophersView

IMPLEMENT_DYNCREATE(CPhilosophersView, CView)

BEGIN_MESSAGE_MAP(CPhilosophersView, CView)
	//{{AFX_MSG_MAP(CPhilosophersView)
	ON_COMMAND(ID_PLAY_ONCE, OnPlayOnce)
	ON_COMMAND(ID_PLAY_MANY, OnPlayMany)
	ON_UPDATE_COMMAND_UI(ID_PLAY_MANY, OnUpdatePlayMany)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhilosophersView construction/destruction

CPhilosophersView::CPhilosophersView()
{
	// TODO: add construction code here
	for(int i = 0; i < PHIL_NUM; i++)
		m_pThread[i] = NULL;
	m_pMutex = new CMutex;
	m_pEvent = new CEvent(FALSE, TRUE);
}

CPhilosophersView::~CPhilosophersView()
{
	delete m_pMutex;
	delete m_pEvent;
}

BOOL CPhilosophersView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CPhilosophersView drawing

void CPhilosophersView::OnDraw(CDC* pDC)
{
	// TODO: add draw code for native data here
	int chopStartX[6], chopStartY[6], chopEndX[6], chopEndY[6];
	int philX[6], philY[6];
	int i;
	CPen pen(PS_SOLID, 3, RGB(0, 0, 0));
	CRect rect = CRect(150, 150, 150+DESK_DIAMETER, 150+DESK_DIAMETER);
	pDC->Ellipse(rect);
	CHOP_COORDINATE(150, 150);
	CPen *pOldPen = (CPen*)pDC->SelectObject(&pen);
	for(i = 0; i < PHIL_NUM; i++)
	{
		pDC->MoveTo(chopStartX[i], chopStartY[i]);
		pDC->LineTo(chopEndX[i], chopEndY[i]);
	}
	pDC->SelectObject(pOldPen);
	PHIL_COORDINATE(150, 150);
	for(i = 0; i< PHIL_NUM; i++)
	{
		pDC->Ellipse(philX[i], philY[i], philX[i] + PHIL_DIAMETER, philY[i] + PHIL_DIAMETER); 
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPhilosophersView printing

BOOL CPhilosophersView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPhilosophersView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPhilosophersView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CPhilosophersView diagnostics

#ifdef _DEBUG
void CPhilosophersView::AssertValid() const
{
	CView::AssertValid();
}

void CPhilosophersView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPhilosophersDoc* CPhilosophersView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPhilosophersDoc)));
	return (CPhilosophersDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPhilosophersView message handlers
void CPhilosophersView::OnPlayOnce() 
{
	// TODO: Add your command handler code here
	int i, left, right;
	int chopStartX[6], chopStartY[6], chopEndX[6], chopEndY[6];
	int philX[6], philY[6];
	PARAM *pParam = NULL;
    
	for(i = 0; i < PHIL_NUM; i++)
	{
		chopSticks[i] = FALSE;
	}
	m_pEvent->ResetEvent();

	CHOP_COORDINATE(150, 150);
	PHIL_COORDINATE(150, 150);
	for(i = 0; i < PHIL_NUM; i++)
	{
		pParam = new PARAM;
		if(NULL == pParam)
		{
			break;
		}
		pParam->Id = i;
		pParam->m_hWnd = this->m_hWnd;
		pParam->philX = philX[i];
		pParam->philY = philY[i];
		left = i;
		right = (i+1) % PHIL_NUM;
		pParam->chopXY[0] = chopStartX[left];
		pParam->chopXY[1] = chopStartY[left];
		pParam->chopXY[2] = chopEndX[left];
		pParam->chopXY[3] = chopEndY[left];
		pParam->chopXY[4] = chopStartX[right];
		pParam->chopXY[5] = chopStartY[right];
		pParam->chopXY[6] = chopEndX[right];
		pParam->chopXY[7] = chopEndY[right];

		m_pThread[i] = AfxBeginThread(DineOnce, (LPVOID)pParam);
	}

	Invalidate();
	m_pEvent->SetEvent();
	pParam = NULL;
}

void CPhilosophersView::OnPlayMany() 
{
	// TODO: Add your command handler code here
	int i, left, right;
	int chopStartX[6], chopStartY[6], chopEndX[6], chopEndY[6];
	int philX[6], philY[6];
	PARAM *pParam = NULL;
    
	for(i = 0; i < PHIL_NUM; i++)
	{
		chopSticks[i] = FALSE;
	}
	for (i =0; i< PHIL_NUM; i++)
	{
		Finished[i] = FALSE;
	}
	m_pEvent->ResetEvent();

	CHOP_COORDINATE(150, 150);
	PHIL_COORDINATE(150, 150);
	for(i = 0; i < PHIL_NUM; i++)
	{
		pParam = new PARAM;
		if(NULL == pParam)
		{
			break;
		}
		pParam->Id = i;
		pParam->m_hWnd = this->m_hWnd;
		pParam->philX = philX[i];
		pParam->philY = philY[i];
		left = i;
		right = (i+1) % PHIL_NUM;
		pParam->chopXY[0] = chopStartX[left];
		pParam->chopXY[1] = chopStartY[left];
		pParam->chopXY[2] = chopEndX[left];
		pParam->chopXY[3] = chopEndY[left];
		pParam->chopXY[4] = chopStartX[right];
		pParam->chopXY[5] = chopStartY[right];
		pParam->chopXY[6] = chopEndX[right];
		pParam->chopXY[7] = chopEndY[right];

		m_pThread[i] = AfxBeginThread(DineMany, (LPVOID)pParam);
	}

	Invalidate();
	m_pEvent->SetEvent();
	pParam = NULL;
}

void CPhilosophersView::OnUpdatePlayMany(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	int i;
	BOOL flag = TRUE;
	for (i =0; i < PHIL_NUM; i++)
	{
		if (FALSE == Finished[i])
		{
			flag = FALSE;
			break;
		}
	}
	pCmdUI->Enable(flag);
}

////////////////////////////////////////////////////////////////
UINT DineOnce(LPVOID pParam)
{
	m_pEvent->Lock();
	PARAM *m_pParam = (PARAM*)pParam;
	int left, right;
	BOOL canDine = FALSE;
	CWnd wnd;
	CDC *pDC;
	CPen pen;
	CBrush brush, *pOldBrush;

	left = m_pParam->Id;
	right = (m_pParam->Id + 1) % PHIL_NUM;

	m_pMutex->Lock();
	if(FALSE == chopSticks[left]
	   && FALSE == chopSticks[right])
	{
		chopSticks[left] = chopSticks[right] = TRUE;
		canDine = TRUE;
	}
	m_pMutex->Unlock();
	
	wnd.Attach(m_pParam->m_hWnd);
    pDC=wnd.GetDC();
	if(TRUE == canDine)
	{
		brush.CreateSolidBrush(RGB(0, 255, 0));
		pOldBrush=(CBrush*)pDC->SelectObject(&brush);
		pDC->Ellipse(m_pParam->philX, m_pParam->philY,
					 m_pParam->philX + PHIL_DIAMETER, m_pParam->philY + PHIL_DIAMETER); 
		pen.CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
		pDC->SelectObject(&pen);
		pDC->MoveTo(m_pParam->philX + PHIL_DIAMETER/2, m_pParam->philY + PHIL_DIAMETER/2);
		pDC->LineTo(m_pParam->chopXY[2], m_pParam->chopXY[3]);
		pDC->MoveTo(m_pParam->philX + PHIL_DIAMETER/2, m_pParam->philY + PHIL_DIAMETER/2);
		pDC->LineTo(m_pParam->chopXY[6], m_pParam->chopXY[7]);
		pDC->SelectObject(pOldBrush);
	}
	else
	{
		brush.CreateSolidBrush(RGB(255, 0, 0));
		pOldBrush=(CBrush*)pDC->SelectObject(&brush);
		pDC->Ellipse(m_pParam->philX, m_pParam->philY,
					 m_pParam->philX + PHIL_DIAMETER, m_pParam->philY + PHIL_DIAMETER); 
		pDC->SelectObject(pOldBrush);
	}
	wnd.Detach();
	delete m_pParam;
	return 0L;
}

UINT DineMany(LPVOID pParam)
{
	m_pEvent->Lock();
	PARAM *m_pParam = (PARAM*)pParam;
	int left, right;
	int TimeDelay = TIME, SleepTime;
	BOOL canDine = FALSE;
	CWnd wnd;
	CDC *pDC;
	CPen pen1, pen2, *pOldPen;
	CBrush brush1, brush2, *pOldBrush;

	left = m_pParam->Id;
	right = (m_pParam->Id + 1) % PHIL_NUM;
	
	wnd.Attach(m_pParam->m_hWnd);
    pDC=wnd.GetDC();
	brush1.CreateSolidBrush(RGB(0, 255, 0));
	brush2.CreateSolidBrush(RGB(255, 0, 0));
	pen1.CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
	pen2.CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
	while(TimeDelay > 0)
	{
		m_pMutex->Lock();
		if(FALSE == chopSticks[left]
			&& FALSE == chopSticks[right])
		{
			chopSticks[left] = chopSticks[right] = TRUE;
			canDine = TRUE;
		}
		m_pMutex->Unlock();
		if(TRUE == canDine)
		{
			
			pOldBrush = (CBrush*)pDC->SelectObject(&brush1);
			pDC->Ellipse(m_pParam->philX, m_pParam->philY,
						m_pParam->philX+PHIL_DIAMETER, m_pParam->philY+PHIL_DIAMETER); 
			
			pDC->SelectObject(&pen1);
			pDC->MoveTo(m_pParam->philX+PHIL_DIAMETER/2, m_pParam->philY + PHIL_DIAMETER/2);
			pDC->LineTo(m_pParam->chopXY[2], m_pParam->chopXY[3]);
			pDC->MoveTo(m_pParam->philX + PHIL_DIAMETER/2, m_pParam->philY + PHIL_DIAMETER/2);
			pDC->LineTo(m_pParam->chopXY[6], m_pParam->chopXY[7]);
			srand((unsigned)time(NULL));
			SleepTime = ((rand() % 250 +250) - (rand() % 250 -250)) / 2;
			Sleep(SleepTime);
			TimeDelay -= SleepTime;
			if(TimeDelay < 0 ) break;
			m_pMutex->Lock();
			chopSticks[left] = chopSticks[right] = FALSE;
			m_pMutex->Unlock();
			canDine = FALSE;
		}
		if(FALSE == canDine)
		{
			
			pOldPen = (CPen*)pDC->SelectObject(&pen2);
			pDC->MoveTo(m_pParam->philX + PHIL_DIAMETER/2, m_pParam->philY + PHIL_DIAMETER/2);
			pDC->LineTo(m_pParam->chopXY[2], m_pParam->chopXY[3]);
			pDC->MoveTo(m_pParam->philX + PHIL_DIAMETER/2, m_pParam->philY + PHIL_DIAMETER/2);
			pDC->LineTo(m_pParam->chopXY[6], m_pParam->chopXY[7]);
			pDC->SelectObject(&brush2);
			pDC->Ellipse(m_pParam->philX, m_pParam->philY,
						m_pParam->philX + PHIL_DIAMETER, m_pParam->philY + PHIL_DIAMETER);		
			pDC->SelectObject(pOldPen);
			srand((unsigned)time(NULL));
			SleepTime = ((rand() % 500 +500) - (rand() % 500 -500)) / 2;
			Sleep(SleepTime);
			TimeDelay -= SleepTime;
		}
	}
	wnd.Detach();
	Finished[m_pParam->Id] = TRUE;
	delete m_pParam;
	return 0L;
}
