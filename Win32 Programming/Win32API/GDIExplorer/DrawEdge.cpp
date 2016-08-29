// DrawEdge.cpp : implementation file
//

#include "stdafx.h"
#include "GDIExplorer.h"
#include "EdgeSamp.h"
#include "drwbutt.h"
#include "DrawEdge.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawEdge dialog


CDrawEdge::CDrawEdge(CWnd* pParent /*=NULL*/)
        : CDialog(CDrawEdge::IDD, pParent)
{
        //{{AFX_DATA_INIT(CDrawEdge)
        //}}AFX_DATA_INIT
}


void CDrawEdge::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CDrawEdge)
        DDX_Control(pDX, IDC_ENABLE, c_Enabled);
        DDX_Control(pDX, IDC_BUTTON, c_Button);
        DDX_Control(pDX, IDC_AUTOERASE, c_AutoErase);
        DDX_Control(pDX, IDC_BF_DIAGONAL_ENDBOTTOMRIGHT, c_DiagonalEndBottomRight);
        DDX_Control(pDX, IDC_BDR_RAISEDINNER, c_RaisedInner);
        DDX_Control(pDX, IDC_BDR_RAISEDOUTER, c_RaisedOuter);
        DDX_Control(pDX, IDC_BDR_SUNKENINNER, c_SunkenInner);
        DDX_Control(pDX, IDC_BDR_SUNKENOUTER, c_SunkenOuter);
        DDX_Control(pDX, IDC_BF_ADJUST, c_Adjust);
        DDX_Control(pDX, IDC_BF_BOTTOM, c_Bottom);
        DDX_Control(pDX, IDC_BF_BOTTOMLEFT, c_BottomLeft);
        DDX_Control(pDX, IDC_BF_BOTTOMRIGHT, c_BottomRight);
        DDX_Control(pDX, IDC_BF_DIAGONAL, c_Diagonal);
        DDX_Control(pDX, IDC_BF_DIAGONAL_ENDBOTTOMLEFT, c_DiagonalEndBottomLeft);
        DDX_Control(pDX, IDC_BF_DIAGONAL_ENDTOPRIGHT, c_DiagonalEndTopRight);
        DDX_Control(pDX, IDC_BF_DIAGONAL_ENDTOPLEFT, c_DiagonalEndTopLeft);
        DDX_Control(pDX, IDC_BF_FLAT, c_Flat);
        DDX_Control(pDX, IDC_BF_LEFT, c_Left);
        DDX_Control(pDX, IDC_BF_MIDDLE, c_Middle);
        DDX_Control(pDX, IDC_BF_MONO, c_Mono);
        DDX_Control(pDX, IDC_BF_RECT, c_Rect);
        DDX_Control(pDX, IDC_BF_RIGHT, c_Right);
        DDX_Control(pDX, IDC_BF_SOFT, c_Soft);
        DDX_Control(pDX, IDC_BF_TOP, c_Top);
        DDX_Control(pDX, IDC_BF_TOPLEFT, c_TopLeft);
        DDX_Control(pDX, IDC_BF_TOPRIGHT, c_TopRight);
        DDX_Control(pDX, IDC_EDGE_BUMP, c_EdgeBump);
        DDX_Control(pDX, IDC_EDGE_ETCHED, c_EdgeEtched);
        DDX_Control(pDX, IDC_EDGE_RAISED, c_EdgeRaised);
        DDX_Control(pDX, IDC_EDGE_SUNKEN, c_EdgeSunken);
        DDX_Control(pDX, IDC_SAMPLE, c_Sample);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDrawEdge, CDialog)
        //{{AFX_MSG_MAP(CDrawEdge)
        ON_BN_CLICKED(IDC_COPY, OnCopy)
        ON_BN_CLICKED(IDC_BDR_RAISEDINNER, OnBdrRaisedinner)
        ON_BN_CLICKED(IDC_BDR_RAISEDOUTER, OnBdrRaisedouter)
        ON_BN_CLICKED(IDC_BDR_SUNKENINNER, OnBdrSunkeninner)
        ON_BN_CLICKED(IDC_BDR_SUNKENOUTER, OnBdrSunkenouter)
        ON_BN_CLICKED(IDC_BF_ADJUST, OnBfAdjust)
        ON_BN_CLICKED(IDC_BF_BOTTOM, OnBfBottom)
        ON_BN_CLICKED(IDC_BF_BOTTOMLEFT, OnBfBottomleft)
        ON_BN_CLICKED(IDC_BF_BOTTOMRIGHT, OnBfBottomright)
        ON_BN_CLICKED(IDC_BF_DIAGONAL, OnBfDiagonal)
        ON_BN_CLICKED(IDC_BF_DIAGONAL_ENDBOTTOMLEFT, OnBfDiagonalEndbottomleft)
        ON_BN_CLICKED(IDC_BF_DIAGONAL_ENDBOTTOMRIGHT, OnBfDiagonalEndbottomright)
        ON_BN_CLICKED(IDC_BF_DIAGONAL_ENDTOPLEFT, OnBfDiagonalEndtopleft)
        ON_BN_CLICKED(IDC_BF_DIAGONAL_ENDTOPRIGHT, OnBfDiagonalEndtopright)
        ON_BN_CLICKED(IDC_BF_FLAT, OnBfFlat)
        ON_BN_CLICKED(IDC_BF_LEFT, OnBfLeft)
        ON_BN_CLICKED(IDC_BF_MIDDLE, OnBfMiddle)
        ON_BN_CLICKED(IDC_BF_MONO, OnBfMono)
        ON_BN_CLICKED(IDC_BF_RECT, OnBfRect)
        ON_BN_CLICKED(IDC_BF_RIGHT, OnBfRight)
        ON_BN_CLICKED(IDC_BF_SOFT, OnBfSoft)
        ON_BN_CLICKED(IDC_BF_TOP, OnBfTop)
        ON_BN_CLICKED(IDC_BF_TOPLEFT, OnBfTopleft)
        ON_BN_CLICKED(IDC_BF_TOPRIGHT, OnBfTopright)
        ON_BN_CLICKED(IDC_EDGE_BUMP, OnEdgeBump)
        ON_BN_CLICKED(IDC_EDGE_ETCHED, OnEdgeEtched)
        ON_BN_CLICKED(IDC_EDGE_RAISED, OnEdgeRaised)
        ON_BN_CLICKED(IDC_EDGE_SUNKEN, OnEdgeSunken)
        ON_BN_CLICKED(IDC_ERASE, OnErase)
        ON_BN_CLICKED(IDC_GETDOWNSTYLE, OnGetdownstyle)
        ON_BN_CLICKED(IDC_GETUPSTYLE, OnGetupstyle)
        ON_BN_CLICKED(IDC_SETDOWNSTYLE, OnSetdownstyle)
        ON_BN_CLICKED(IDC_SETUPSTYLE, OnSetupstyle)
        ON_BN_CLICKED(IDC_B, OnB)
        ON_BN_CLICKED(IDC_HC, OnHc)
        ON_BN_CLICKED(IDC_L, OnL)
        ON_BN_CLICKED(IDC_R, OnR)
        ON_BN_CLICKED(IDC_T, OnT)
        ON_BN_CLICKED(IDC_VC, OnVc)
        ON_BN_CLICKED(IDC_ENABLE, OnEnable)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawEdge message handlers


/****************************************************************************
*                           CDrawEdge::eraseSample
* Result: void
*       
* Effect: 
*       Erases the sample
* Notes:
*       "Erasing the sample" is not the same as invalidating the rectangle
*       of the client area of the sample control and causing it to repaint!
*       In particular, only the relevant areas of the parent are redrawn,
*       and any pixels in the parent area not repainted explicitly (such as
*       diagonals when the diagonal style is removed) remain visible on the
*       parent.  Here we are repainting the area of the parent of the control,
*       not the control itself!
****************************************************************************/

void CDrawEdge::eraseSample()
    {
     CBrush br(::GetSysColor(COLOR_3DFACE));
     CClientDC dc(this);

     CRect r;
     c_Sample.GetWindowRect(&r);
     ScreenToClient(&r);

     dc.SelectObject(&br);
     dc.FillRect(&r, &br);

     c_Sample.InvalidateRect(NULL);
    }

/****************************************************************************
*                            CDrawEdge::getStyles
* Result: DWORD
*       style flags, with the edge in the low-order 16 bits and the
*       border in the high-order 16 bits.
****************************************************************************/

DWORD CDrawEdge::getStyles()
    {
     int edge = 0;
     int border = 0;
     if(c_RaisedInner.GetCheck())
        edge |= BDR_RAISEDINNER;
     if(c_RaisedOuter.GetCheck())
        edge |= BDR_RAISEDOUTER;
     if(c_SunkenInner.GetCheck())
        edge |= BDR_SUNKENINNER;
     if(c_SunkenOuter.GetCheck())
        edge |= BDR_SUNKENOUTER;

     if(c_Adjust.GetCheck())
        border |= BF_ADJUST;
     if(c_Bottom.GetCheck())
        border |= BF_BOTTOM;
     if(c_BottomLeft.GetCheck())
        border |= BF_BOTTOMLEFT;
     if(c_BottomRight.GetCheck())
        border |= BF_BOTTOMRIGHT;
     if(c_Diagonal.GetCheck())
        border |= BF_DIAGONAL;
     if(c_DiagonalEndBottomLeft.GetCheck())
        border |= BF_DIAGONAL_ENDBOTTOMLEFT;
     if(c_DiagonalEndBottomRight.GetCheck())
        border |= BF_DIAGONAL_ENDBOTTOMRIGHT;
     if(c_DiagonalEndTopLeft.GetCheck())
        border |= BF_DIAGONAL_ENDTOPLEFT;
     if(c_DiagonalEndTopRight.GetCheck())
        border |= BF_DIAGONAL_ENDTOPRIGHT;
     if(c_Flat.GetCheck())
        border |= BF_FLAT;
     if(c_Left.GetCheck())
        border |= BF_LEFT;
     if(c_Middle.GetCheck())
        border |= BF_MIDDLE;
     if(c_Mono.GetCheck())
        border |= BF_MONO;
     if(c_Rect.GetCheck())
        border |= BF_RECT;
     if(c_Right.GetCheck())
        border |= BF_RIGHT;
     if(c_Soft.GetCheck())
        border |= BF_SOFT;
     if(c_Top.GetCheck())
        border |= BF_TOP;
     if(c_TopLeft.GetCheck())
        border |= BF_TOPLEFT;
     if(c_TopRight.GetCheck())
        border |= BF_TOPRIGHT;
     
     return MAKELONG(edge, border);
    }

/****************************************************************************
*                              CDrawEdge::redraw
* Result: void
*       
* Effect: 
*       Draws an edge using DrawEdge
****************************************************************************/

void CDrawEdge::redraw()
    {
     if(c_AutoErase.GetCheck())
        eraseSample();
        
     ::SetWindowLong(c_Sample.m_hWnd, GWL_USERDATA, getStyles());

     c_Sample.InvalidateRect(NULL);
    }

void CDrawEdge::OnBdrRaisedinner() 
{
 setEdge();
 redraw();
}

void CDrawEdge::OnBdrRaisedouter() 
{
 setEdge();
 redraw();
}

void CDrawEdge::OnBdrSunkeninner() 
{
 setEdge();
 redraw();
}

void CDrawEdge::OnBdrSunkenouter() 
{
 setEdge();
 redraw();
}

void CDrawEdge::OnBfAdjust() 
{
 redraw();
}

void CDrawEdge::OnBfBottom() 
{
 setComposite();
 redraw();
}

void CDrawEdge::OnBfBottomleft() 
{
 setBase();
 redraw();
}

void CDrawEdge::OnBfBottomright() 
{
 setBase();
 redraw();
}

void CDrawEdge::OnBfDiagonal() 
{
 setComposite();
 redraw();
}

void CDrawEdge::OnBfDiagonalEndbottomleft() 
{
 setBase();
 redraw();
}

void CDrawEdge::OnBfDiagonalEndbottomright() 
{
 setBase();
 redraw();      
}

void CDrawEdge::OnBfDiagonalEndtopleft() 
{
 setBase();
 redraw();
}

void CDrawEdge::OnBfDiagonalEndtopright() 
{
 setBase();
 redraw();
}

void CDrawEdge::OnBfFlat() 
{
 redraw();
}

void CDrawEdge::OnBfLeft() 
{
 setComposite();
 redraw();
}

void CDrawEdge::OnBfMiddle() 
{
 redraw();
}

void CDrawEdge::OnBfMono() 
{
 redraw();
}

void CDrawEdge::OnBfRect() 
{
 setBase();
 redraw();
}

void CDrawEdge::OnBfRight() 
{
 setComposite();
 redraw();
}

void CDrawEdge::OnBfSoft() 
{
 redraw();
}

void CDrawEdge::OnBfTop() 
{
 setComposite();
 redraw();
}

void CDrawEdge::OnBfTopleft() 
{
 setBase();
 redraw();
}

void CDrawEdge::OnBfTopright() 
{
 setBase();
 redraw();
}

void CDrawEdge::OnEdgeBump() 
{
 // Sets BDR_RAISEDOUTER, BDR_SUNKENINNER

 CheckRadioButton(IDC_BDR_RAISEDOUTER, IDC_BDR_SUNKENOUTER, IDC_BDR_RAISEDOUTER);
 CheckRadioButton(IDC_BDR_RAISEDINNER, IDC_BDR_SUNKENINNER, IDC_BDR_SUNKENINNER);
 redraw();
}

void CDrawEdge::OnEdgeEtched() 
{
 // Sets BDR_SUNKENOUTER, BDR_RAISEDINNER

 CheckRadioButton(IDC_BDR_RAISEDOUTER, IDC_BDR_SUNKENOUTER, IDC_BDR_SUNKENOUTER);
 CheckRadioButton(IDC_BDR_RAISEDINNER, IDC_BDR_SUNKENINNER, IDC_BDR_RAISEDINNER);
 redraw();
}

void CDrawEdge::OnEdgeRaised() 
{
 // Sets BDR_RAISEDOUTER, BDR_RAISEDINNER

 CheckRadioButton(IDC_BDR_RAISEDOUTER, IDC_BDR_SUNKENOUTER, IDC_BDR_RAISEDOUTER);
 CheckRadioButton(IDC_BDR_RAISEDINNER, IDC_BDR_SUNKENINNER, IDC_BDR_RAISEDINNER);
 redraw();
}

void CDrawEdge::OnEdgeSunken() 
{
 // Sets BDR_SUNKENOUTER, BDR_SUNKENINNER

 CheckRadioButton(IDC_BDR_RAISEDOUTER, IDC_BDR_SUNKENOUTER, IDC_BDR_SUNKENOUTER);
 CheckRadioButton(IDC_BDR_RAISEDINNER, IDC_BDR_SUNKENINNER, IDC_BDR_SUNKENINNER);
 redraw();
}

/****************************************************************************
*                             CDrawEdge::setEdge
* Result: void
*       
* Effect: 
*       Sets the 'edge' radio button based on the inner/outer settings
****************************************************************************/

void CDrawEdge::setEdge()
{
 if(c_RaisedInner.GetCheck() && c_RaisedOuter.GetCheck())
    CheckRadioButton(IDC_EDGE_BUMP, IDC_EDGE_SUNKEN, IDC_EDGE_RAISED);
 if(c_SunkenInner.GetCheck() && c_RaisedOuter.GetCheck())
    CheckRadioButton(IDC_EDGE_BUMP, IDC_EDGE_SUNKEN, IDC_EDGE_BUMP);
 if(c_RaisedInner.GetCheck() && c_SunkenOuter.GetCheck())
    CheckRadioButton(IDC_EDGE_BUMP, IDC_EDGE_SUNKEN, IDC_EDGE_ETCHED);
 if(c_SunkenInner.GetCheck() && c_SunkenOuter.GetCheck())
    CheckRadioButton(IDC_EDGE_BUMP, IDC_EDGE_SUNKEN, IDC_EDGE_SUNKEN);
}


BOOL CDrawEdge::OnInitDialog() 
{
        CDialog::OnInitDialog();
        
        CheckRadioButton(IDC_BDR_RAISEDINNER, IDC_BDR_SUNKENINNER, IDC_BDR_SUNKENINNER);
        CheckRadioButton(IDC_BDR_RAISEDOUTER, IDC_BDR_SUNKENOUTER, IDC_BDR_SUNKENOUTER);
        setEdge();
        
        CheckRadioButton(IDC_T, IDC_B, IDC_VC);
        setVStyle(BS_VCENTER);

        CheckRadioButton(IDC_L, IDC_R, IDC_HC);
        setHStyle(BS_CENTER);

        c_Rect.SetCheck(TRUE);
        setBase();

        c_Enabled.SetCheck(TRUE);

        redraw();   // force bits to be attached to child window

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CDrawEdge::OnErase() 
{
 eraseSample();
        
}

/****************************************************************************
*                             CDrawEdge::setBase
* Result: void
*       
* Effect: 
*       Given the current composite operation, set the base flags that 
*       represent it
* Notes:
*       BF_TOPLEFT                  (BF_TOP | BF_LEFT)
*       BF_TOPRIGHT                 (BF_TOP | BF_RIGHT)
*       BF_BOTTOMLEFT               (BF_BOTTOM | BF_LEFT)
*       BF_BOTTOMRIGHT              (BF_BOTTOM | BF_RIGHT)
*       BF_RECT                     (BF_LEFT | BF_TOP | BF_RIGHT | BF_BOTTOM)
*       BF_DIAGONAL_ENDTOPRIGHT     (BF_DIAGONAL | BF_TOP | BF_RIGHT)
*       BF_DIAGONAL_ENDTOPLEFT      (BF_DIAGONAL | BF_TOP | BF_LEFT)
*       BF_DIAGONAL_ENDBOTTOMLEFT   (BF_DIAGONAL | BF_BOTTOM | BF_LEFT)
*       BF_DIAGONAL_ENDBOTTOMRIGHT  (BF_DIAGONAL | BF_BOTTOM | BF_RIGHT)
****************************************************************************/

void CDrawEdge::setBase()
    {
     c_Diagonal.SetCheck(c_DiagonalEndTopRight.GetCheck() |
                         c_DiagonalEndTopLeft.GetCheck()  |
                         c_DiagonalEndBottomLeft.GetCheck() |
                         c_DiagonalEndBottomRight.GetCheck());

     c_Top.SetCheck(     c_DiagonalEndTopRight.GetCheck() |
                         c_DiagonalEndTopLeft.GetCheck() |
                         c_TopLeft.GetCheck() |
                         c_TopRight.GetCheck() |
                         c_Rect.GetCheck());

     c_Bottom.SetCheck(  c_DiagonalEndBottomLeft.GetCheck() |
                         c_DiagonalEndBottomRight.GetCheck() |
                         c_BottomLeft.GetCheck() |
                         c_BottomRight.GetCheck() |
                         c_Rect.GetCheck());
     
     c_Left.SetCheck(    c_DiagonalEndTopLeft.GetCheck() |
                         c_DiagonalEndBottomLeft.GetCheck() |
                         c_TopLeft.GetCheck() |
                         c_BottomLeft.GetCheck() |
                         c_Rect.GetCheck());

     c_Right.SetCheck(   c_DiagonalEndTopRight.GetCheck() |
                         c_DiagonalEndBottomRight.GetCheck() |
                         c_TopRight.GetCheck() |
                         c_BottomRight.GetCheck() |
                         c_Rect.GetCheck() );
                         
    }

/****************************************************************************
*                           CDrawEdge::setComposite
* Result: void
*       
* Effect: 
*       Sets the composite flags based on the settings of the base flags
****************************************************************************/

void CDrawEdge::setComposite()
    {
     c_TopLeft.SetCheck(    c_Top.GetCheck() && c_Left.GetCheck());

     c_TopRight.SetCheck(   c_Top.GetCheck() && c_Right.GetCheck());

     c_BottomLeft.SetCheck( c_Bottom.GetCheck() && c_Left.GetCheck());

     c_BottomRight.SetCheck(c_Bottom.GetCheck() && c_Right.GetCheck());

     c_Rect.SetCheck(       c_Top.GetCheck() && c_Left.GetCheck() &&
                            c_Bottom.GetCheck() && c_Right.GetCheck());

     c_DiagonalEndTopRight.SetCheck(c_Diagonal.GetCheck() && 
                                    c_Top.GetCheck() && c_Right.GetCheck());

     c_DiagonalEndTopLeft.SetCheck(c_Diagonal.GetCheck() && 
                                    c_Top.GetCheck() && c_Left.GetCheck());

     c_DiagonalEndBottomRight.SetCheck(c_Diagonal.GetCheck() && 
                                    c_Bottom.GetCheck() && c_Right.GetCheck());

     c_DiagonalEndBottomLeft.SetCheck(c_Diagonal.GetCheck() && 
                                    c_Bottom.GetCheck() && c_Left.GetCheck());
    }

/****************************************************************************
*                              CDrawEdge::OnCopy
* Result: void
*       
* Effect: 
*       Creates a new bitmap, copies the contents of the target bitmap to it,
*       and adds the resulting bitmap to the clipboard.
* Notes:
*       The coordinates transferred are based on the origin and width
*       parameters for the destintation bitmap.
****************************************************************************/


void CDrawEdge::OnCopy() 
    {
     CClientDC sourceDC(this);

     // Create a bitmap into which we copy the current dest bitmap
     CDC memDC;
     memDC.CreateCompatibleDC(&sourceDC);

     CBitmap bm;
     CRect r;

     c_Sample.GetWindowRect(&r);
     ScreenToClient(&r);

     r.InflateRect(5,5);

     CPoint org(r.left, r.top);
     CSize ext(r.Width(), r.Height());

     bm.CreateCompatibleBitmap(&sourceDC, ext.cx, ext.cy);

     CBitmap * oldbm = memDC.SelectObject(&bm);

     memDC.BitBlt(0, 0, ext.cx, ext.cy, &sourceDC, org.x, org.y, SRCCOPY);
 
     OpenClipboard();
     ::EmptyClipboard();
     ::SetClipboardData(CF_BITMAP, bm.m_hObject);
     CloseClipboard();

     memDC.SelectObject(oldbm);
     bm.Detach();  // make sure bitmap not deleted with CBitmap object
        
}


void CDrawEdge::OnGetdownstyle() 
{
        
}

void CDrawEdge::OnGetupstyle() 
{

}

void CDrawEdge::OnSetdownstyle() 
{
 DWORD styles = getStyles();
 c_Button.setDownStyle(styles);
}

void CDrawEdge::OnSetupstyle() 
{
 DWORD styles = getStyles();
 c_Button.setUpStyle(styles);
 c_Button.InvalidateRect(NULL);  // force redraw of up-state
}



void CDrawEdge::setHStyle(DWORD newstyle)
{
 DWORD style = c_Button.GetStyle();
 style &= ~(BS_LEFT | BS_CENTER | BS_RIGHT);
 style |= newstyle;
 ::SetWindowLong(c_Button.m_hWnd, GWL_STYLE, style);
 c_Button.InvalidateRect(NULL);
}

void CDrawEdge::OnHc() 
{
 setHStyle(BS_CENTER);
}

void CDrawEdge::OnL() 
{
 setHStyle(BS_LEFT);
        
}

void CDrawEdge::OnR() 
{
 setHStyle(BS_RIGHT);
}


void CDrawEdge::setVStyle(DWORD newstyle)
{
 DWORD style = c_Button.GetStyle();
 style &= ~(BS_TOP | BS_VCENTER | BS_BOTTOM);
 style |= newstyle;
 ::SetWindowLong(c_Button.m_hWnd, GWL_STYLE, style);
 c_Button.InvalidateRect(NULL);
}

void CDrawEdge::OnB() 
{
 setVStyle(BS_BOTTOM);
}

void CDrawEdge::OnT() 
{
 setVStyle(BS_TOP);
}

void CDrawEdge::OnVc() 
{
 setVStyle(BS_VCENTER);
        
}

void CDrawEdge::OnEnable() 
{
 c_Button.EnableWindow(c_Enabled.GetCheck());
}
