// sdemo.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "NumericEdit.h"
#include "drawing.h"
#include "sdemo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSliderDemo dialog


CSliderDemo::CSliderDemo(CWnd* pParent /*=NULL*/)
        : CDialog(CSliderDemo::IDD, pParent)
{
        initialized = FALSE;

        //{{AFX_DATA_INIT(CSliderDemo)
        //}}AFX_DATA_INIT
}


void CSliderDemo::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CSliderDemo)
        DDX_Control(pDX, IDC_PICTURE, c_Picture);
        DDX_Control(pDX, IDC_SPINVALUE, c_SpinValue);
        DDX_Control(pDX, IDC_YSLIDER, c_YSlider);
        DDX_Control(pDX, IDC_XSLIDER, c_XSlider);
        DDX_Control(pDX, IDC_VSCROLL, c_VScroll);
        DDX_Control(pDX, IDC_VALUE, c_Value);
        DDX_Control(pDX, IDC_PROGRESS, c_Progress);
        DDX_Control(pDX, IDC_HSCROLL, c_HScroll);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSliderDemo, CDialog)
        //{{AFX_MSG_MAP(CSliderDemo)
        ON_WM_HSCROLL()
        ON_WM_VSCROLL()
        ON_EN_CHANGE(IDC_VALUE, OnChangeValue)
        ON_WM_DESTROY()
        ON_WM_TIMER()
        //}}AFX_MSG_MAP
        ON_REGISTERED_MESSAGE(CDrawing::UWM_QUERY_XOFFSET, OnQueryXOffset)
        ON_REGISTERED_MESSAGE(CDrawing::UWM_QUERY_YOFFSET, OnQueryYOffset)
        ON_REGISTERED_MESSAGE(CDrawing::UWM_QUERY_SIZE,    OnQuerySize)
        ON_REGISTERED_MESSAGE(CDrawing::UWM_QUERY_PENSIZE, OnQueryPenSize)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSliderDemo message handlers

BOOL CSliderDemo::OnInitDialog() 
{
        CDialog::OnInitDialog();
        
        CRect r;
        c_Picture.GetClientRect(&r);

        c_XSlider.SetRange(0, r.Width());
        c_XSlider.SetPos(r.Width() / 10);
        c_XSlider.SetTicFreq(r.Width() / 10);
        
        c_YSlider.SetRange(0, r.Height());
        c_YSlider.SetPos(r.Height() / 10);
        c_YSlider.SetTicFreq( r.Height() / 10);

        c_HScroll.SetScrollRange(0, r.Width());
        c_HScroll.SetScrollPos(r.Width() / 2);

        c_VScroll.SetScrollRange(0, r.Height());
        c_VScroll.SetScrollPos(r.Height() / 2);

        c_SpinValue.SetRange(1, 10);
        c_SpinValue.SetPos(1);

        progress = 0;
        limit = 100;
        c_Progress.SetRange(0, limit);
        delta = 1;
        timer = SetTimer(1, 50, NULL);
        
        initialized = TRUE;

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CSliderDemo::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
        if(pScrollBar->m_hWnd == c_HScroll.m_hWnd)
           { /* vertical scroll */
            setScroll(nSBCode, nPos, pScrollBar);
            c_Picture.InvalidateRect(NULL, TRUE);
           } /* vertical scroll */
        else
        if(pScrollBar->m_hWnd == c_XSlider.m_hWnd)
           c_Picture.InvalidateRect(NULL, TRUE);

        
        CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

/****************************************************************************
*                           CSliderDemo::setScroll
* Inputs:
*       UINT nSBCode:
*       UINT nPos:
*       CScrollBar * pScrollBar:
* Result: void
*       
* Effect: 
*       Updates the scrollbar
****************************************************************************/

void CSliderDemo::setScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar)
    {
     int pos = pScrollBar->GetScrollPos();
     
     switch(nSBCode)
        { /* decode */
         case SB_LINELEFT:
                 if(pos > 0)
                    pos--;
                 break;
         case SB_LINERIGHT:
                 if(pos < pScrollBar->GetScrollLimit())
                    pos++;
                 break;
         case SB_PAGELEFT:
                 pos -= pScrollBar->GetScrollLimit() / 10;
                 if(pos < 0)
                    pos = 0;
                 break;
         case SB_PAGERIGHT:
                 pos += pScrollBar->GetScrollLimit() / 10;
                 if(pos > pScrollBar->GetScrollLimit())
                    pos = pScrollBar->GetScrollLimit();
                 break;
         case SB_THUMBTRACK:
                 pos = nPos;
                 break;
         case SB_THUMBPOSITION:
                 pos = nPos;
                 break;
        } /* decode */
     pScrollBar->SetScrollPos(pos);
    }

void CSliderDemo::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
        if(pScrollBar->m_hWnd == c_VScroll.m_hWnd)
           { /* vertical scroll */
            setScroll(nSBCode, nPos, pScrollBar);
            
            c_Picture.InvalidateRect(NULL, TRUE);
           } /* vertical scroll */
        else
        if(pScrollBar->m_hWnd == c_YSlider.m_hWnd)
           c_Picture.InvalidateRect(NULL, TRUE);
        
        CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CSliderDemo::OnChangeValue() 
{
        if(!initialized)
           return;  // not everything is there yet...

        delta = c_Value.GetWindowInt();
}

void CSliderDemo::OnCancel() 
{
        // TODO: Add extra cleanup here
        
        CDialog::OnCancel();
}

void CSliderDemo::OnDestroy() 
{
        CDialog::OnDestroy();
        
        KillTimer(timer);
        initialized = FALSE;
}

void CSliderDemo::OnTimer(UINT nIDEvent) 
{
        
        progress += delta;
        if(delta > 0 && progress > limit ||
           delta < 0 && progress < 0)
              { /* change direction */
               if(delta < 0)
                  progress = 0;
               else
                  progress = limit;

               delta = -delta;
              } /* change direction */

        c_Progress.SetPos(progress);
        CDialog::OnTimer(nIDEvent);
}

/****************************************************************************
*                         CSliderDemo::OnQueryXOffset
* Inputs:
*       WPARAM: ignored
*       LPARAM: ignored
* Result: LRESULT
*       X-offset value
****************************************************************************/

LRESULT CSliderDemo::OnQueryXOffset(WPARAM, LPARAM)
    {
     return c_HScroll.GetScrollPos();
    }

/****************************************************************************
*                         CSliderDemo::OnQueryYoffset
* Inputs:
*       WPARAM: ignored
*       LPARAM: ignored
* Result: LRESULT
*       Y-offset value
****************************************************************************/

LRESULT CSliderDemo::OnQueryYOffset(WPARAM, LPARAM)
    {
     return c_VScroll.GetScrollPos();
    }

/****************************************************************************
*                         CSliderDemo::OnQuerySize
* Inputs:
*       WPARAM: ignored
*       LPARAM: (LPARAM)(LPRECT) rp
* Result: LRESULT
*       0, always
****************************************************************************/

LRESULT CSliderDemo::OnQuerySize(WPARAM, LPARAM rp)
    {
     ((LPRECT)rp)->left = 0;
     ((LPRECT)rp)->top = 0;
     ((LPRECT)rp)->right = c_XSlider.GetPos();
     ((LPRECT)rp)->bottom = c_YSlider.GetPos();
     return 0;
    }

/****************************************************************************
*                          CSliderDemo::OnQueryPenSize
* Inputs:
*       WPARAM: ignored
*       LPARAM: ignored
* Result: LRESULT
*       Pen width
****************************************************************************/

LRESULT CSliderDemo::OnQueryPenSize(WPARAM, LPARAM)
    {
     return c_Value.GetWindowInt();
    }
