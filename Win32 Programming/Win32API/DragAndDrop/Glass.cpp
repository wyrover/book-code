// Glass.cpp : implementation file
//

#include "stdafx.h"
#include "dipper.h"
#include "Glass.h"
#include "bucket.h"
#include "query.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int levels[] = 
		{ IDI_EMPTY_GLASS, IDI_1_GLASS, IDI_2_GLASS, IDI_3_GLASS};

#define NUM_LEVELS ((sizeof(levels) / sizeof(levels[0])))

/////////////////////////////////////////////////////////////////////////////
// CGlass

CGlass::CGlass()
{
 filled = 0;
}

CGlass::~CGlass()
{
}


BEGIN_MESSAGE_MAP(CGlass, CStatic)
	//{{AFX_MSG_MAP(CGlass)
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(UWM_DoDrop, dropIt)
	ON_REGISTERED_MESSAGE(UWM_QueryDrop, queryDrop)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGlass message handlers

/****************************************************************************
*                               CGlass::dropIt
* Inputs:
*       WPARAM: unused
*	LPARAM: unused
* Result: LRESULT
*       TRUE, always
* Effect: 
*       Drops the contents of the dipper into the glass.  Displays the new
*	glass
****************************************************************************/

LRESULT CGlass::dropIt(WPARAM, LPARAM)
    {
     if(filled < NUM_LEVELS - 1)
        { /* can fill */
	 filled++;
	 SetIcon(AfxGetApp()->LoadIcon(levels[filled]));
	} /* can fill */
     return TRUE;
    }

/****************************************************************************
*                              CGlass::queryDrop
* Inputs:
*       WPARAM: ignored
*	LPARAM: ignored
* Result: LRESULT
*       0 - drop state unknown (never returned by this function, just by
*			DefWindowProc)
*	QUERY_DROP_OK: we can drop into this glass
*	QUERY_DROP_FULL: glass is full
****************************************************************************/

LRESULT CGlass::queryDrop(WPARAM, LPARAM)
    {
     return (filled < NUM_LEVELS - 1) ? QUERY_DROP_OK : QUERY_DROP_FULL;
    }


UINT CGlass::OnNcHitTest(CPoint point) 
{
 	return HTCLIENT;
}

BOOL CGlass::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
 	return 0; // don't change the current cursor
}
