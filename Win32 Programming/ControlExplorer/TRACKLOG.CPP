// tracklog.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "ControlExplorer.h"
#include "NumericEdit.h"
#include "tracklog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogarithmic dialog


CLogarithmic::CLogarithmic(CWnd* pParent /*=NULL*/)
        : CDialog(CLogarithmic::IDD, pParent)
{
        recursion = FALSE;

        //{{AFX_DATA_INIT(CLogarithmic)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}


void CLogarithmic::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CLogarithmic)
        DDX_Control(pDX, IDC_VALUE, c_Value);
        DDX_Control(pDX, IDC_TRACKBAR, c_Trackbar);
        DDX_Control(pDX, IDC_POS, c_Pos);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogarithmic, CDialog)
        //{{AFX_MSG_MAP(CLogarithmic)
        ON_WM_HSCROLL()
        ON_EN_CHANGE(IDC_POS, OnChangePos)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogarithmic message handlers


/****************************************************************************
*                           CLogarithmic::setValue
* Result: void
*       
* Effect: 
*       Sets the "value" field based on the physical position.  This
*       requires computing the value represented by the logarithmic
*       calibration.
* Notes:
*       Assumes "low" is 0, otherwise we need to do full range computation
*       below.  We cheat for expediency.
*
*                   [ pos * log10(hi)  ]
*                   [ ---------------- ]
*                   [        hi        ]
*          val = 10
*
****************************************************************************/

void CLogarithmic::setValue()
    {

        int pos = c_Trackbar.GetPos();
        recursion = TRUE;
        c_Pos.SetWindowText(pos);
        recursion = FALSE;

        //          [           pos ]
        //          [ log(ul) * --- ]
        //          [           ul  ]
        // val=  10

        int val = (int)
            (pow(10.0, 
                 (double)(log10((double)c_Trackbar.GetRangeMax())
                        * (double)pos) / 
                              (double)(c_Trackbar.GetRangeMax())) + 0.5);
        
        c_Value.SetWindowText(val);
     
    }

void CLogarithmic::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
        setValue();
        CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CLogarithmic::OnInitDialog() 
{
        CDialog::OnInitDialog();
        
        c_Trackbar.SetRangeMin(0);
        c_Trackbar.SetRangeMax(100);
        c_Trackbar.SetPageSize(10);

        int high = c_Trackbar.GetRangeMax();
        int page = c_Trackbar.GetPageSize();

        // Assume low is 0!

        double logupper = log10((double)high);

        for(int i = page; i < high; i += page)
           { /* set ticks */
            int tick = (int)((double)high * log10((double)i) / logupper + 0.5); // low == 0!
            c_Trackbar.SetTic(tick);
           } /* set ticks */
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CLogarithmic::OnChangePos() 
{
 if(recursion)
    return;

 c_Trackbar.SetPos(c_Pos.GetWindowInt());

 // We don't have to worry about user typing garbage like negative numbers
 // if we let the trackbar control normalize it for us...
 setValue();
        
}
