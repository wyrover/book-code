// Rop.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "idcombo.h"
#include "PatRop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static IDData extrops[] = {
/* 00 */        {IDS_0,         0x00000042},
/* 05 */        {IDS_DPon,      0x000500A9},
/* 0A */        {IDS_DPna,      0x000A0329},
/* 0F */        {IDS_Pn,        0x000F0001},
/* 50 */        {IDS_PDna,      0x00500325},
/* 55 */        {IDS_Dn,        0x00550009},
/* 5A */        {IDS_DPx,       0x005A0049},
/* 5F */        {IDS_DPan,      0x005F00E9},
/* A0 */        {IDS_DPa,       0x00A000C9},
/* A5 */        {IDS_PDxn,      0x00A50065},
/* AA */        {IDS_D,         0x00AA0029},
/* AF */        {IDS_DPno,      0x00AF0229},
/* F0 */        {IDS_P,         0x00F00021},
/* F5 */        {IDS_PDno,      0x00F50225},
/* FA */        {IDS_DPo,       0x00FA0089},
/* FF */        {IDS_1,         0x00FF0062},
                { 0, 0} // EOF
                        };

static IDData rops[] = {
        { IDS_BLACKNESS, BLACKNESS},
        { IDS_DSTINVERT, DSTINVERT},
        { IDS_PATCOPY, PATCOPY},
        { IDS_PATINVERT, PATINVERT},
        { IDS_WHITENESS, WHITENESS},
        { 0, 0} // EOT
                       };

/////////////////////////////////////////////////////////////////////////////
// CPatRop

CPatRop::CPatRop()
{
 maxlen = -1;
}

CPatRop::~CPatRop()
{
}


BEGIN_MESSAGE_MAP(CPatRop, CIDCombo)
        //{{AFX_MSG_MAP(CPatRop)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatRop message handlers

/****************************************************************************
*                                 CPatRop::Load
* Inputs:
*       BOOL ext: TRUE for extended opcodes, FALSE for normal opcodes
*       int def: Default value
* Result: int
*       Selected object
* Effect: 
*       Loads the combo box and makes the initial selection
****************************************************************************/

int CPatRop::Load(BOOL ext, int def)
    {
     ResetContent();
     return CIDCombo::AddStrings(ext ? extrops : rops, def);
    }
