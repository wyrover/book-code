// GenBlt.cpp : implementation file
//

#include "stdafx.h"
#include "GDIExplorer.h"
#include "source.h"
#include "dest.h"
#include "IntEdit.h"
#include "idcombo.h"
#include "gcombo.h"
#include "image.h"
#include "bmselect.h"
#include "bmw.h"
#include "strmode.h"
#include "brdemo.h"

#include "blt.h"
#include "GenBlt.h"
#include "is95.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGenBlt property page

IMPLEMENT_DYNCREATE(CGenBlt, CBlt)

CGenBlt::CGenBlt() 
{
 // should never be called.
}
            

CGenBlt::CGenBlt(int id) : CBlt(id)
{
        //{{AFX_DATA_INIT(CGenBlt)
        //}}AFX_DATA_INIT
}

CGenBlt::~CGenBlt()
{
}

void CGenBlt::DoDataExchange(CDataExchange* pDX)
{
        CBlt::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CGenBlt)
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGenBlt, CBlt)
        //{{AFX_MSG_MAP(CGenBlt)
        ON_EN_CHANGE(IDC_PLGX, OnChangePlgx)
        ON_EN_CHANGE(IDC_PLGY, OnChangePlgy)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGenBlt message handlers

BOOL CGenBlt::OnInitDialog() 
{
        CBlt::OnInitDialog();
        
        // Set the GenBlt parameters for an identity transformation
        //
        //      [ {0, 0}, {width, 0}, {0, height} ]

        updateSource();  // we require the source rectangle be established

        CSize sz = getSourceBitmapSize();
        
        CRect r;
        c_Dest.getObjectRect(&r);
        c_PlgXSpin.SetRange(-r.Width(), r.Width());
        c_PlgYSpin.SetRange(-r.Width(), r.Height());

        c_ulX.SetVal(0);                // point[0].x
        c_ulY.SetVal(0);                // point[0].y
        c_PlgX.SetVal(sz.cx);           // point[1].x
        c_PlgY.SetVal(0);               // point[1].y
        c_lrX.SetVal(0);                // point[2].x
        c_lrY.SetVal(sz.cy);            // point[2].y

        endInitDialog();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

/****************************************************************************
*                             CGenBlt::hasSource
* Result: BOOL
*       TRUE, always 
****************************************************************************/

BOOL CGenBlt::hasSource()
    {
     return TRUE;
    }

/****************************************************************************
*                           CGenBlt::updateControls
* Result: void
*       
* Effect: 
*       Adjusts values of the controls.  This includes:
*               - Setting the limits for the source bitmap controls
****************************************************************************/

void CGenBlt::updateControls()
    {
     CBlt::updateControls();
     setLastError();
    }


/****************************************************************************
*                            CGenBlt::setLastError
* Result: void
*       
* Effect: 
*       Displays the result of the "lasterror" code in the error message
*       box.
****************************************************************************/

void CGenBlt::setLastError()
    {
     CBlt::setLastError();

     if(lasterror == 0)
        { /* maybe warn */
         // We issue a warning here if the user selects one of the StretchBlt
         // modes that doesn't work on Windows 95 (see KB article Q138105)
         switch(getStrMode())
            { /* warn user */
             case STRETCH_ANDSCANS:
             case STRETCH_ORSCANS:
             case STRETCH_HALFTONE:
                     if(Is95())
                        { /* bogus */
                         CString fmt;
                         fmt.LoadString(IDS_WIN95_BOGUS);
                         CString s;
                         s.Format(fmt, getStrModeStr());
                         c_Error.SetWindowText(s);
                        } /* bogus */
                     break;
             case STRETCH_DELETESCANS:
                     if(Is95())
                        c_Error.SetWindowText(_T(""));
                     break;
            } /* warn user */
        } /* maybe warn */
     
    }

void CGenBlt::OnChangePlgx() 
{
 updateControls();
 makeOp();
}

void CGenBlt::OnChangePlgy() 
{
 updateControls();
 makeOp();
        
}

void CGenBlt::OnChangeXmask() 
{
 makeOp();

}

void CGenBlt::OnChangeYmask() 
{
 makeOp();
        
}

/****************************************************************************
*                             CGenBlt::getStrMode
* Result: int
*       0, always (implement in subclass)
* Effect: 
*       ASSERT(FALSE)
****************************************************************************/

int CGenBlt::getStrMode()
    {
     ASSERT(FALSE);  // implement in subclass
     return 0;  // bogus value returned
    }

/****************************************************************************
*                             CGenBlt::getStrModeStr
* Result: CString
*       "", always (implement in subclass)
* Effect: 
*       ASSERT(FALSE)
****************************************************************************/

CString CGenBlt::getStrModeStr()
    {
     ASSERT(FALSE);  // implement in subclass
     return CString(_T(""));  // bogus value returned
    }
