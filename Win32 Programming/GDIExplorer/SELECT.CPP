// select.cpp : implementation file
//

#include "stdafx.h"
#include "randstat.h"

#include "GDIExplorer.h"
#include "select.h"
#include "random.h"
#include "is95.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelect dialog


/****************************************************************************
*                                CSelect::Is32s
* Result: BOOL
*       TRUE if running on Win32s
*       FALSE if running on Win/NT or Win95
****************************************************************************/

BOOL CSelect::Is32s()
    {
     OSVERSIONINFO info;
     info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
     ::GetVersionEx(&info);
     return (info.dwPlatformId == VER_PLATFORM_WIN32s);
    }

CSelect::CSelect(CWnd* pParent /*=NULL*/)
        : CDialog(CSelect::IDD, pParent)
{
        //{{AFX_DATA_INIT(CSelect)
        //}}AFX_DATA_INIT
}


void CSelect::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CSelect)
        DDX_Control(pDX, IDC_ANGLEARC, c_AngleArc);
        DDX_Control(pDX, IDC_GEOPEN, c_GeoPen);
        DDX_Control(pDX, IDC_TEXTXFORM, c_TextXForm);
        DDX_Control(pDX, IDC_XFORM, c_XForm);
        DDX_Control(pDX, IDC_ARCTO, c_ArcTo);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelect, CDialog)
        //{{AFX_MSG_MAP(CSelect)
        ON_BN_CLICKED(IDC_BEZIER, OnBezier)
        ON_BN_CLICKED(IDC_ENDCAPS, OnEndcaps)
        ON_BN_CLICKED(IDC_LINEJOIN, OnLinejoin)
        ON_BN_CLICKED(IDC_TEXTCLIP1, OnTextClip1)
        ON_BN_CLICKED(IDC_XFORM, OnXform)
        ON_BN_CLICKED(IDC_TEXTXFORM, OnTextxform)
        ON_BN_CLICKED(IDC_ANGLEARC, OnAnglearc)
        ON_BN_CLICKED(IDC_ARC, OnArc)
        ON_BN_CLICKED(IDC_ARCTO, OnArcTo)
        ON_BN_CLICKED(IDC_ELLIPSE, OnEllipse)
        ON_BN_CLICKED(IDC_CHORD, OnChord)
        ON_BN_CLICKED(IDC_PIE, OnPie)
        ON_BN_CLICKED(IDC_RECTANGLE, OnRectangle)
        ON_BN_CLICKED(IDC_ROUNDRECT, OnRoundrect)
        ON_BN_CLICKED(IDC_PENSTYLES, OnPenstyles)
        ON_BN_CLICKED(IDC_POLYGON, OnPolygon)
        ON_BN_CLICKED(IDC_DRAWFOCUS, OnDrawfocus)
        ON_BN_CLICKED(IDC_HATCHBRUSH, OnHatchbrush)
        ON_BN_CLICKED(IDC_STROKEANDFILL, OnStrokeandfill)
        ON_BN_CLICKED(IDC_TEXTCLIP2, OnTextclip2)
        ON_BN_CLICKED(IDC_TEXTCLIP3, OnTextclip3)
        ON_BN_CLICKED(IDC_COMPLEX_PATH, OnComplexPath)
        ON_BN_CLICKED(IDC_RANDOM, OnRandom)
        ON_BN_CLICKED(IDC_BMPBRUSH, OnBmpbrush)
        ON_BN_CLICKED(IDC_GEOPEN, OnGeopen)
        ON_BN_CLICKED(IDC_ENDCAPS_STROKED, OnEndcapsStroked)
        ON_BN_CLICKED(IDC_POLYPOLYGON, OnPolypolygon)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSelect message handlers

void CSelect::OnBezier() 
{
 style = IDC_BEZIER;
 CDialog::OnOK();       
}

void CSelect::OnEndcaps() 
{
 style = IDC_ENDCAPS;
 CDialog::OnOK();       
        
}

void CSelect::OnLinejoin() 
{
 style = IDC_LINEJOIN;
 CDialog::OnOK();       
        
}

void CSelect::OnTextClip1() 
{
 style = IDC_TEXTCLIP1;
 CDialog::OnOK();       
        
}

void CSelect::OnXform() 
{
 style = IDC_XFORM;
 CDialog::OnOK();
        
}


void CSelect::OnTextxform() 
{
 style = IDC_TEXTXFORM;
 CDialog::OnOK();
        
}

void CSelect::OnAnglearc() 
{
 style = IDC_ANGLEARC;
 CDialog::OnOK();

}

void CSelect::OnArc() 
{
 style = IDC_ARC;
 CDialog::OnOK();
        
}

void CSelect::OnArcTo() 
{
 style = IDC_ARCTO;
 CDialog::OnOK();
        
}


void CSelect::OnEllipse() 
{
 style = IDC_ELLIPSE;
 CDialog::OnOK();
}

void CSelect::OnChord() 
{
 style = IDC_CHORD;
 CDialog::OnOK();
}

void CSelect::OnPie() 
{
 style = IDC_PIE;
 CDialog::OnOK();
}

void CSelect::OnRectangle() 
{
 style = IDC_RECTANGLE;
 CDialog::OnOK();
}

void CSelect::OnRoundrect() 
{
 style = IDC_ROUNDRECT;
 CDialog::OnOK();
}

void CSelect::OnPenstyles() 
{
 style = IDC_PENSTYLES;
 CDialog::OnOK();
}

void CSelect::OnPolygon() 
{
 style = IDC_POLYGON;
 CDialog::OnOK();
}

void CSelect::OnDrawfocus() 
{
 style = IDC_DRAWFOCUS;
 CDialog::OnOK();
}

void CSelect::OnHatchbrush() 
{
 style = IDC_HATCHBRUSH;
 CDialog::OnOK();

}

void CSelect::OnStrokeandfill() 
{
    style = IDC_STROKEANDFILL;
    CDialog::OnOK();
}

BOOL CSelect::OnInitDialog() 
{
        CDialog::OnInitDialog();
        
        CheckRadioButton(IDC_BEZIER, IDC_UNIMP29, style);
        
        // Any controls that were disabled in the template are made
        // to disappear; these are the "unimplemented" controls for
        // future expansion.

        for(int i = IDC_BEZIER; i <= IDC_UNIMP29; i++)
           { /* hide disabled */
            CWnd * w = GetDlgItem(i);
            if(w == NULL)
               continue;
            if(!w->IsWindowEnabled())
              w->ShowWindow(SW_HIDE);
           } /* hide disabled */

        // Platform-specific disables: Don't enable what Win95 doesn't
        // support

        BOOL enable = !(Is95() || Is32s());

        c_GeoPen.EnableWindow(enable);
        c_TextXForm.EnableWindow(enable);
        c_XForm.EnableWindow(enable);
        c_AngleArc.EnableWindow(enable);
        c_ArcTo.EnableWindow(enable);

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}



void CSelect::OnTextclip2() 
{
 style = IDC_TEXTCLIP2;
 CDialog::OnOK();       
        
}

void CSelect::OnTextclip3() 
{
 style = IDC_TEXTCLIP3;
 CDialog::OnOK();       
}

void CSelect::OnComplexPath() 
{
 style = IDC_COMPLEX_PATH;
 CDialog::OnOK();
        
}

void CSelect::OnRandom() 
{
 CRandom dlg;
 dlg.rd = rd;
 if(dlg.DoModal() == IDOK)
    { /* success */
     rd = dlg.rd;
     style = IDC_RANDOM;
     CDialog::OnOK();
    } /* success */
}

void CSelect::OnBmpbrush() 
{
 style = IDC_BMPBRUSH;
 CDialog::OnOK();
        
}

void CSelect::OnGeopen() 
{
 style = IDC_GEOPEN;
 CDialog::OnOK();       
        
}

void CSelect::OnEndcapsStroked() 
{
 style = IDC_ENDCAPS_STROKED;
 CDialog::OnOK();
        
}


void CSelect::OnPolypolygon() 
{
 style = IDC_POLYPOLYGON;
 CDialog::OnOK();
        
}
