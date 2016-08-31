// RopCodes.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "IDCombo.h"
#include "RopCodes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static IDData rops[] = {
	{IDS_R2_BLACK,       R2_BLACK},
	{IDS_R2_NOTMERGEPEN, R2_NOTMERGEPEN},
	{IDS_R2_MASKNOTPEN,  R2_MASKNOTPEN},
	{IDS_R2_NOTCOPYPEN,  R2_NOTCOPYPEN},
	{IDS_R2_MASKPENNOT,  R2_MASKPENNOT},
	{IDS_R2_NOT,	     R2_NOT},
	{IDS_R2_XORPEN,	     R2_XORPEN},
	{IDS_R2_NOTMASKPEN,  R2_NOTMASKPEN},
	{IDS_R2_MASKPEN,     R2_MASKPEN},
	{IDS_R2_NOTXORPEN,   R2_NOTXORPEN},
        {IDS_R2_NOP,         R2_NOP},
	{IDS_R2_MERGENOTPEN, R2_MERGENOTPEN},
	{IDS_R2_COPYPEN,     R2_COPYPEN},
	{IDS_R2_MERGEPENNOT, R2_MERGEPENNOT},
	{IDS_R2_MERGEPEN,    R2_MERGEPEN},
	{IDS_R2_WHITE,       R2_WHITE},
	{ 0, 0 } // EOT

		       };
/////////////////////////////////////////////////////////////////////////////
// CRopCodes

CRopCodes::CRopCodes()
{
}

CRopCodes::~CRopCodes()
{
}


BEGIN_MESSAGE_MAP(CRopCodes, CIDCombo)
	//{{AFX_MSG_MAP(CRopCodes)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRopCodes message handlers

/****************************************************************************
*                               CRopCodes::Load
* Inputs:
*       int sel: Selection to make after loading
* Result: void
*       
* Effect: 
*       Loads the control with the rop codes
****************************************************************************/

void CRopCodes::Load(int sel)
    {
     AddStrings(rops);
     Select(sel);
    }
