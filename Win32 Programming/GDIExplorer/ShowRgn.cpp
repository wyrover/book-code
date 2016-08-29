// ShowRgn.cpp : implementation file
//

#include "stdafx.h"
#include "GDIExplorer.h"
#include "rgndisp.h"
#include "rgnint.h"
#include "rgncont.h"
#include "ShowRgn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowRegion dialog


CShowRegion::CShowRegion(CWnd* pParent /*=NULL*/)
        : CDialog(CShowRegion::IDD, pParent)
{
        //{{AFX_DATA_INIT(CShowRegion)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}


void CShowRegion::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CShowRegion)
        DDX_Control(pDX, IDC_MONO, c_Monochrome);
        DDX_Control(pDX, IDC_INTERSECT, c_Intersected);
        DDX_Control(pDX, IDC_CONTAINED, c_Contained);
        DDX_Control(pDX, IDC_FRAMERGN, c_FrameRgn);
        DDX_Control(pDX, IDC_FILLRGN, c_FillRgn);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShowRegion, CDialog)
        //{{AFX_MSG_MAP(CShowRegion)
        ON_BN_CLICKED(IDC_RGN_1, OnRgn1)
        ON_BN_CLICKED(IDC_RGN_2, OnRgn2)
        ON_BN_CLICKED(IDC_RGN_AND, OnRgnAnd)
        ON_BN_CLICKED(IDC_RGN_DIFF, OnRgnDiff)
        ON_BN_CLICKED(IDC_RGN_OR, OnRgnOr)
        ON_BN_CLICKED(IDC_RGN_RESULT, OnRgnResult)
        ON_BN_CLICKED(IDC_RGN_XOR, OnRgnXor)
        ON_BN_CLICKED(IDC_FILLRGN, OnFillrgn)
        ON_BN_CLICKED(IDC_FRAMERGN, OnFramergn)
        ON_BN_CLICKED(IDC_COPY1, OnCopy1)
        ON_BN_CLICKED(IDC_COPY2, OnCopy2)
        ON_BN_CLICKED(IDC_MONO, OnMono)
        //}}AFX_MSG_MAP
        ON_MESSAGE(UWM_QUERY_RGNWHAT, OnRgnWhat)
        ON_MESSAGE(UWM_QUERY_RGNOP, OnRgnOp)
        ON_MESSAGE(UWM_QUERY_RGNHOW, OnRgnHow)
        ON_MESSAGE(UWM_QUERY_MONO, OnRgnMono)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowRegion message handlers

void CShowRegion::update()
{
 c_Intersected.InvalidateRect(NULL);
 c_Contained.InvalidateRect(NULL);
}

void CShowRegion::OnRgn1() 
{
 which = RGN_1;
 update();
}

void CShowRegion::OnRgn2() 
{
 which = RGN_2;
 update();
}

void CShowRegion::OnRgnAnd() 
{
 op = RGN_AND;
 update();      
}

void CShowRegion::OnRgnDiff() 
{
 op = RGN_DIFF;
 update();      
}

void CShowRegion::OnRgnOr() 
{
 op = RGN_OR;
 update();
}

void CShowRegion::OnRgnResult() 
{
 which = RGN_RESULT;
 update();
}

void CShowRegion::OnRgnXor() 
{
 op = RGN_XOR;
 update();
        
}

BOOL CShowRegion::OnInitDialog() 
{
        CDialog::OnInitDialog();
        
        c_Intersected.initialize();
        c_Contained.initialize();

        which = RGN_RESULT;
        CheckRadioButton(IDC_RGN_1, IDC_RGN_RESULT, IDC_RGN_RESULT);

        how = RGN_FILL;
        c_FillRgn.SetCheck(TRUE);
        c_FrameRgn.SetCheck(FALSE);
        
        op = RGN_OR;
        CheckRadioButton(IDC_RGN_AND, IDC_RGN_XOR, IDC_RGN_OR);

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CShowRegion::OnFillrgn() 
{
 if(c_FillRgn.GetCheck())
    how |= RGN_FILL;
 else
    how &= ~RGN_FILL;
 update();      
}

void CShowRegion::OnFramergn() 
{
 if(c_FrameRgn.GetCheck())
    how |= RGN_FRAME;
 else
    how &= ~RGN_FRAME;
 update();
}

LRESULT CShowRegion::OnRgnHow(WPARAM, LPARAM)
{
 return how;
}

LRESULT CShowRegion::OnRgnMono(WPARAM, LPARAM)
{
 return c_Monochrome.GetCheck();
}

LRESULT CShowRegion::OnRgnWhat(WPARAM, LPARAM)
{
 return which;
}

LRESULT CShowRegion::OnRgnOp(WPARAM, LPARAM)
{
 return op;
}

void CShowRegion::OnCopy1() 
{
 doCopy(&c_Intersected);
        
}

void CShowRegion::OnCopy2() 
{
 doCopy(&c_Contained);
        
}

/****************************************************************************
*                             CShowRegion::doCopy
* Result: void
*       
* Effect: 
*       Creates a new bitmap, copies the contents of the target bitmap to it,
*       and adds the resulting bitmap to the clipboard.
* Notes:
*       The coordinates transferred are based on the origin and width
*       parameters for the destintation bitmap.
****************************************************************************/


void CShowRegion::doCopy(CWnd * wnd) 
    {
     CClientDC dc(wnd);

     // Create a bitmap into which we copy the current dest bitmap
     CDC memDC;
     memDC.CreateCompatibleDC(&dc);

     CBitmap bm;
     CRect r;

     wnd->GetClientRect(&r);

     bm.CreateCompatibleBitmap(&dc, r.Width(), r.Height());

     CBitmap * oldbm = memDC.SelectObject(&bm);

     memDC.BitBlt(0, 0, r.Width(), r.Height(), &dc, 0, 0, SRCCOPY);
 
     OpenClipboard();
     ::EmptyClipboard();
     ::SetClipboardData(CF_BITMAP, bm.m_hObject);
     CloseClipboard();

     memDC.SelectObject(oldbm);
     bm.Detach();  // make sure bitmap not deleted with CBitmap object
        
}

void CShowRegion::OnMono() 
{
 update();
}
