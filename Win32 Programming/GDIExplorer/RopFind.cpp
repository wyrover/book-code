// RopFind.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "idcombo.h"
#include "bitrop.h"
#include "box.h"
#include "RopFind.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRopFinder property page

IMPLEMENT_DYNCREATE(CRopFinder, CPropertyPage)

CRopFinder::CRopFinder() : CPropertyPage(CRopFinder::IDD)
{
        //{{AFX_DATA_INIT(CRopFinder)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CRopFinder::~CRopFinder()
{
}

void CRopFinder::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CRopFinder)
        DDX_Control(pDX, IDC_ROPCODE, c_RopCode);
        DDX_Control(pDX, IDC_S7, c_S7);
        DDX_Control(pDX, IDC_S6, c_S6);
        DDX_Control(pDX, IDC_S5, c_S5);
        DDX_Control(pDX, IDC_S4, c_S4);
        DDX_Control(pDX, IDC_S3, c_S3);
        DDX_Control(pDX, IDC_S2, c_S2);
        DDX_Control(pDX, IDC_S1, c_S1);
        DDX_Control(pDX, IDC_S0, c_S0);
        DDX_Control(pDX, IDC_ROP, c_Rop);
        DDX_Control(pDX, IDC_R7, c_R7);
        DDX_Control(pDX, IDC_R6, c_R6);
        DDX_Control(pDX, IDC_R5, c_R5);
        DDX_Control(pDX, IDC_R4, c_R4);
        DDX_Control(pDX, IDC_R3, c_R3);
        DDX_Control(pDX, IDC_R2, c_R2);
        DDX_Control(pDX, IDC_R1, c_R1);
        DDX_Control(pDX, IDC_R0, c_R0);
        DDX_Control(pDX, IDC_P7, c_P7);
        DDX_Control(pDX, IDC_P6, c_P6);
        DDX_Control(pDX, IDC_P5, c_P5);
        DDX_Control(pDX, IDC_P4, c_P4);
        DDX_Control(pDX, IDC_P3, c_P3);
        DDX_Control(pDX, IDC_P2, c_P2);
        DDX_Control(pDX, IDC_P1, c_P1);
        DDX_Control(pDX, IDC_P0, c_P0);
        DDX_Control(pDX, IDC_D7, c_D7);
        DDX_Control(pDX, IDC_D6, c_D6);
        DDX_Control(pDX, IDC_D5, c_D5);
        DDX_Control(pDX, IDC_D4, c_D4);
        DDX_Control(pDX, IDC_D3, c_D3);
        DDX_Control(pDX, IDC_D2, c_D2);
        DDX_Control(pDX, IDC_D1, c_D1);
        DDX_Control(pDX, IDC_D0, c_D0);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRopFinder, CPropertyPage)
        //{{AFX_MSG_MAP(CRopFinder)
        ON_BN_CLICKED(IDC_R0, OnR0)
        ON_BN_CLICKED(IDC_R1, OnR1)
        ON_BN_CLICKED(IDC_R2, OnR2)
        ON_BN_CLICKED(IDC_R3, OnR3)
        ON_BN_CLICKED(IDC_R4, OnR4)
        ON_BN_CLICKED(IDC_R5, OnR5)
        ON_BN_CLICKED(IDC_R6, OnR6)
        ON_BN_CLICKED(IDC_R7, OnR7)
        ON_CBN_SELENDOK(IDC_ROP, OnSelendokRop)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRopFinder message handlers

void CRopFinder::OnR0() 
{
 c_R0.toggle();
 setRop();
        
}

void CRopFinder::OnR1() 
{
 c_R1.toggle();
 setRop();
        
}

void CRopFinder::OnR2() 
{
 c_R2.toggle();
 setRop();
        
}

void CRopFinder::OnR3() 
{
 c_R3.toggle();
 setRop();
        
}

void CRopFinder::OnR4() 
{
 c_R4.toggle();
 setRop();
        
}

void CRopFinder::OnR5() 
{
 c_R5.toggle();
 setRop();
        
}

void CRopFinder::OnR6() 
{
 c_R6.toggle();
 setRop();
        
}

void CRopFinder::OnR7() 
{
 c_R7.toggle();
 setRop();
        
}

void CRopFinder::OnSelendokRop() 
{
 updateResult();
}

BOOL CRopFinder::OnInitDialog() 
{
        CPropertyPage::OnInitDialog();
        
        c_P7.SetCheck(TRUE);
        c_P6.SetCheck(TRUE);
        c_P5.SetCheck(TRUE);
        c_P4.SetCheck(TRUE);

        c_S7.SetCheck(TRUE);
        c_S6.SetCheck(TRUE);
        c_S3.SetCheck(TRUE);
        c_S2.SetCheck(TRUE);

        c_D7.SetCheck(TRUE);
        c_D5.SetCheck(TRUE);
        c_D3.SetCheck(TRUE);
        c_D1.SetCheck(TRUE);
        
        //     Pattern: 11110000
        //      Source: 11001100
        // Destination: 10101010

        c_Rop.Load(TRUE);
        c_Rop.Select(SRCCOPY);

        updateResult();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

/****************************************************************************
*                          CRopFinder::updateResult
* Result: void
*       
* Effect: 
*       Updates the bit pattern in the destination check boxes, based on 
*       the current value of the c_Rop.
****************************************************************************/

void CRopFinder::updateResult()
    {
     DWORD rop = c_Rop.GetCurItem();

     // 0x00**????   <= ** represents the rop code

     setResult(LOBYTE(HIWORD(rop)));

     setRopcode();
    }

/****************************************************************************
*                            CRopFinder::setResult
* Inputs:
*       BYTE rop: 8-bit ROP code
* Result: void
*       
* Effect: 
*       Transfers the ROP code to the result flags
****************************************************************************/

void CRopFinder::setResult(BYTE rop)
    {
     c_R7.SetCheck(rop & 0x80 ? 1 : 0);
     c_R6.SetCheck(rop & 0x40 ? 1 : 0);
     c_R5.SetCheck(rop & 0x20 ? 1 : 0);
     c_R4.SetCheck(rop & 0x10 ? 1 : 0);
     c_R3.SetCheck(rop & 0x08 ? 1 : 0);
     c_R2.SetCheck(rop & 0x04 ? 1 : 0);
     c_R1.SetCheck(rop & 0x02 ? 1 : 0);
     c_R0.SetCheck(rop & 0x01 ? 1 : 0);

    }

/****************************************************************************
*                            CRopFinder::getResult
* Result: BYTE
*       8-bit result code based on check boxes
****************************************************************************/

BYTE CRopFinder::getResult()
    {
     return
        (c_R7.GetCheck() ? 0x80 : 0) |
        (c_R6.GetCheck() ? 0x40 : 0) |
        (c_R5.GetCheck() ? 0x20 : 0) |
        (c_R4.GetCheck() ? 0x10 : 0) |
        (c_R3.GetCheck() ? 0x08 : 0) |
        (c_R2.GetCheck() ? 0x04 : 0) |
        (c_R1.GetCheck() ? 0x02 : 0) |
        (c_R0.GetCheck() ? 0x01 : 0);
    }

/****************************************************************************
*                           CRopFinder::setRopcode
* Result: void
*       
* Effect: 
*       Changes the 8-digit hex raster op
****************************************************************************/

void CRopFinder::setRopcode()
    {
     DWORD rop = c_Rop.GetCurItem();
     CString s;
     s.Format(_T("0x%08x"), rop);
     c_RopCode.SetWindowText(s);
    }

/****************************************************************************
*                             CRopFinder::setRop
* Result: void
*       
* Effect: 
*       Given the values in the result code, selects a ROP code
* Notes:
*       This depends on the fact that the combo box is in numeric order
*       of rop codes.
****************************************************************************/

void CRopFinder::setRop()
    {
     BYTE result = getResult();

     c_Rop.SetCurSel((int)result);

     setRopcode();
    }
