// Panel.cpp : implementation file
//

#include "stdafx.h"
#include <afxmt.h>
#include "SynchronizationExplorer.h"
#include "thread.h"
#include "delta.h"
#include "Panel.h"
#include "uwm.h"
#include "childfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HWND CPanel::panel = NULL;

/////////////////////////////////////////////////////////////////////////////
// CPanel

IMPLEMENT_DYNCREATE(CPanel, CFormView)

CPanel::CPanel()
        : CFormView(CPanel::IDD)
{
        loops = 0;
        //{{AFX_DATA_INIT(CPanel)
        //}}AFX_DATA_INIT
}

CPanel::~CPanel()
{
}

void CPanel::DoDataExchange(CDataExchange* pDX)
{
        CFormView::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CPanel)
        DDX_Control(pDX, IDC_SYNCHRONOUS, c_Synchronous);
        DDX_Control(pDX, IDC_SPEED_DISPLAY, c_SpeedDisplay);
        DDX_Control(pDX, IDC_SPEED, c_Speed);
        DDX_Control(pDX, IDC_RANDOM, c_Random);
        DDX_Control(pDX, IDC_N, c_N);
        DDX_Control(pDX, IDC_LOCKING, c_Locking);
        DDX_Control(pDX, IDC_ERRORBAR, c_ErrorBar);
        DDX_Control(pDX, IDC_ACTUAL, c_Actual);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPanel, CFormView)
        //{{AFX_MSG_MAP(CPanel)
        ON_BN_CLICKED(IDC_LOCKING, OnLocking)
        ON_BN_CLICKED(IDC_RANDOM, OnRandom)
        ON_WM_VSCROLL()
        ON_BN_CLICKED(IDC_SYNCHRONOUS, OnSynchronous)
        ON_MESSAGE(UWM_N, OnN)
        ON_MESSAGE(UWM_LOOP, OnLoop)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanel diagnostics

#ifdef _DEBUG
void CPanel::AssertValid() const
{
        CFormView::AssertValid();
}

void CPanel::Dump(CDumpContext& dc) const
{
        CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPanel message handlers

void CPanel::OnLocking() 
{
 CThread::setLock(c_Locking.GetCheck());
        
}

void CPanel::OnRandom() 
{
 CThread::setRandom(c_Random.GetCheck());
        
}

#define UNITS     20    // Multiplier for range
#define MIN_SPEED UNITS // shortest speed x 1ms
#define MAX_SPEED 1000  // longest  speed x 1ms
#define INIT_SPEED 100  // default  speed x 1ms

/****************************************************************************
*                              CPanel::showSpeed
* Inputs:
*       int pos: Value to display
* Result: void
*       
* Effect: 
*       Displays the value
****************************************************************************/

void CPanel::showSpeed(int pos)
    {
     CString s;
     s.Format(_T("%d"), pos * UNITS);
     c_SpeedDisplay.SetWindowText(s);     
    }

void CPanel::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
        if(pScrollBar->m_hWnd == c_Speed.m_hWnd)
           { /* set speed */
            int pos = c_Speed.GetPos();
            switch(nSBCode)
               { /* nSBCode */
                case SB_LINEUP:
                        pos += UNITS;
                        break;
                case SB_LINEDOWN:
                        pos -= UNITS;
                        break;
                case SB_PAGEUP:
                        pos += (MAX_SPEED / UNITS) / 10;  // 10% range up
                        break;
                case SB_PAGEDOWN:
                        pos -= (MAX_SPEED / UNITS) / 10;  // 10% range down
                        break;
                case SB_THUMBTRACK:
                        pos = nPos;
                        break;
                case SB_THUMBPOSITION:
                        pos = nPos;
                        break;
                case SB_TOP:
                        pos = MAX_SPEED;
                        break;
                case SB_BOTTOM:
                        pos = 0;
                        break;
                case SB_ENDSCROLL:
                        break;
               } /* nSBCode */
            c_Speed.SetPos(pos);
            CThread::setDelay(pos * UNITS);
            showSpeed(pos);
           } /* set speed */
        
        CFormView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPanel::OnInitialUpdate() 
{
        CFormView::OnInitialUpdate();
        
        GetParentFrame()->RecalcLayout();
        
        ResizeParentToFit(TRUE);
        ResizeParentToFit(FALSE);

        ((CChildFrame *)GetParentFrame())->setSize();

        c_Speed.SetRange(0, MAX_SPEED / UNITS);
        int pos = INIT_SPEED / UNITS;
        c_Speed.SetPos(pos);
        CThread::setDelay(pos * UNITS);
        showSpeed(pos);

        c_Locking.SetCheck(TRUE);
        OnLocking();

        // Store the handle for the notification messages
        panel = m_hWnd;

}

/****************************************************************************
*                                 CPanel::OnN
* Inputs:
*       WPARAM wParam: Current value of counter
*       LPARAM: unused
* Result: LRESULT
*       logically void, 0, always
* Effect: 
*       Displays the current counter value
****************************************************************************/

LRESULT CPanel::OnN(WPARAM wParam, LPARAM)
    {
     CString s;
     s.Format(_T("%d"), wParam);
     c_N.SetWindowText(s);
     c_ErrorBar.actual = wParam;
     c_ErrorBar.InvalidateRect(NULL, TRUE);
     return 0;
    }

/****************************************************************************
*                               CPanel::OnLoop
* Inputs:
*       WPARAM: unused
*       LPARAM: unused
* Result: LRESULT
*       logically void, 0, always
* Effect: 
*       Updates the loop count and displays it
****************************************************************************/

LRESULT CPanel::OnLoop(WPARAM, LPARAM)
    {
     loops++;
     CString s;
     s.Format(_T("%d"), loops);
     c_Actual.SetWindowText(s);
     c_ErrorBar.loops = loops;
     c_ErrorBar.InvalidateRect(NULL,TRUE);
     return 0;
    }

void CPanel::OnSynchronous() 
{
        CThread::setMessaging(c_Synchronous.GetCheck() 
                                        ? MESSAGING_SYNCHRONOUS
                                        : MESSAGING_ASYNCHRONOUS);
        
}

