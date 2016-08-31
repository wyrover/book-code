// CStretchBltOp.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "idcombo.h"
#include "strmode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static IDData rops[] = {
        { IDS_STRETCH_ANDSCANS, STRETCH_ANDSCANS},
        { IDS_STRETCH_ORSCANS, STRETCH_ORSCANS},
        { IDS_STRETCH_DELETESCANS, STRETCH_DELETESCANS},
        { IDS_STRETCH_HALFTONE, STRETCH_HALFTONE},
        { 0, 0 } // EOT

                       };

/////////////////////////////////////////////////////////////////////////////
// CStretchBltOp

CStretchBltOp::CStretchBltOp()
{
 maxlen = -1;
}

CStretchBltOp::~CStretchBltOp()
{
}


BEGIN_MESSAGE_MAP(CStretchBltOp, CIDCombo)
        //{{AFX_MSG_MAP(CStretchBltOp)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStretchBltOp message handlers

/****************************************************************************
*                                 CStretchBltOp::Load
* Inputs:
*       BOOL ext: TRUE for extended opcodes, FALSE for normal opcodes
*       int def: Default value
* Result: int
*       Selected object
* Effect: 
*       Loads the combo box and makes the initial selection
****************************************************************************/

int CStretchBltOp::Load(int def)
    {
     ResetContent();
     return CIDCombo::AddStrings(rops, def);
    }
