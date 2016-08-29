// random.cpp : implementation file
//

#include "stdafx.h"
#include "randstat.h"

#include "GDIExplorer.h"
#include "random.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRandom dialog


CRandom::CRandom(CWnd* pParent /*=NULL*/)
        : CDialog(CRandom::IDD, pParent)
{
        rd.seed = 0;
        rd.sides = 3;
        rd.RandomSides = FALSE;
        rd.fillmode = WINDING;
        //{{AFX_DATA_INIT(CRandom)
        //}}AFX_DATA_INIT
}


void CRandom::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CRandom)
        DDX_Control(pDX, IDC_SIDES, c_Sides);
        DDX_Control(pDX, IDC_RANDOM_SIDES, c_RandomSides);
        DDX_Control(pDX, IDC_SPINSIDES, c_SpinSides);
        //}}AFX_DATA_MAP
        DDX_Text(pDX, IDC_SEED, rd.seed);
        DDX_Check(pDX, IDC_RANDOM_SIDES, rd.RandomSides);
        DDX_Text(pDX, IDC_SIDES, rd.sides);
}


BEGIN_MESSAGE_MAP(CRandom, CDialog)
        //{{AFX_MSG_MAP(CRandom)
        ON_BN_CLICKED(IDC_RANDOM_SIDES, OnRandomSides)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRandom message handlers

BOOL CRandom::OnInitDialog() 
    {
     if(rd.seed == 0)
        rd.seed = 12345;

     CDialog::OnInitDialog();

     c_SpinSides.SetRange(3,40);

     c_SpinSides.EnableWindow(!rd.RandomSides);
     c_Sides.EnableWindow(!rd.RandomSides);

     switch(rd.fillmode)
        { /* set fill mode */
         case ALTERNATE:
                 CheckDlgButton(IDC_ALTERNATE, TRUE);
                 break;
         case WINDING:
                 CheckDlgButton(IDC_WINDING, TRUE);
                 break;
         default:
                 CheckDlgButton(IDC_NONE, TRUE);
                 break;
        } /* set fill mode */
        
     return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
    }

void CRandom::OnOK() 
    {
     if(IsDlgButtonChecked(IDC_WINDING))
        rd.fillmode = WINDING;
     else
     if(IsDlgButtonChecked(IDC_ALTERNATE))
        rd.fillmode = ALTERNATE;
     else
        rd.fillmode = (WINDING + ALTERNATE) ; // bogus value

     CDialog::OnOK();

     if(!rd.RandomSides && rd.sides < 3)
        rd.sides = 3;
    }

void CRandom::OnRandomSides() 
{
     c_SpinSides.EnableWindow(!IsDlgButtonChecked(IDC_RANDOM_SIDES));
     c_Sides.EnableWindow(!IsDlgButtonChecked(IDC_RANDOM_SIDES));
        
}
