// DevCapa.cpp : implementation file
//

#include "stdafx.h"
#include "PrintExplorer.h"
#include "bitcombo.h"
#include "numericedit.h"
#include "message.h"
#include "handleedit.h"
#include "devnames.h"
#include "hdevmode.h"
#include "enumedit.h"
#include "scombo.h"
#include "pdata.h"
#include "pname.h"
#include "papers.h"
#include "psizes.h"

#include "DevCapa.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeviceCapabilities property page

IMPLEMENT_DYNCREATE(CDeviceCapabilities, CPropertyPage)

CDeviceCapabilities::CDeviceCapabilities() : CPropertyPage(CDeviceCapabilities::IDD)
{
        pd = NULL;
        //{{AFX_DATA_INIT(CDeviceCapabilities)
        //}}AFX_DATA_INIT
}

CDeviceCapabilities::~CDeviceCapabilities()
{
}

void CDeviceCapabilities::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CDeviceCapabilities)
        DDX_Control(pDX, IDC_DC_PAPERNAMES, c_DC_PAPERNAMES);
        DDX_Control(pDX, IDC_DC_PAPERSIZE, c_DC_PAPERSIZE);
        DDX_Control(pDX, IDC_DC_PAPERS, c_DC_PAPERS);
        DDX_Control(pDX, IDC_c_DC_PAPERS, c_c_DC_PAPERS);
        DDX_Control(pDX, IDC_c_DC_PAPERSIZE, c_c_DC_PAPERSIZE);
        DDX_Control(pDX, IDC_c_DC_PAPERNAMES, c_c_DC_PAPERNAMES);
        DDX_Control(pDX, IDC_DC_FILEDEPENDENCIES, c_DC_FILEDEPENDENCIES);
        DDX_Control(pDX, IDC_c_DC_FILEDEPENDENCIES, c_c_DC_FILEDEPENDENCIES);
        DDX_Control(pDX, IDC_DC_DATATYPE_PRODUCED, c_DC_DATATYPE_PRODUCED);
        DDX_Control(pDX, IDC_c_DC_DATATYPE_PRODUCED, c_c_DC_DATATYPE_PRODUCED);
        DDX_Control(pDX, IDC_c_DC_BINS, c_c_DC_BINS);
        DDX_Control(pDX, IDC_c_DC_BINNAMES, c_c_DC_BINNAMES);
        DDX_Control(pDX, IDC_c_DC_ENUMRESOLUTIONS, c_c_DC_ENUMRESOLUTIONS);
        DDX_Control(pDX, IDC_DC_BINS, c_DC_BINS);
        DDX_Control(pDX, IDC_DC_BINNAMES, c_DC_BINNAMES);
        DDX_Control(pDX, IDC_DC_ENUMRESOLUTIONS, c_DC_ENUMRESOLUTIONS);
        DDX_Control(pDX, IDC_c_DC_BINADJUST, c_c_DC_BINADJUST);
        DDX_Control(pDX, IDC_DC_EMF_COMPLIANT, c_DC_EMF_COMPLIANT);
        DDX_Control(pDX, IDC_DC_BINADJUST, c_DC_BINADJUST);
        DDX_Control(pDX, IDC_DC_FIELDS, c_DC_FIELDS);
        DDX_Control(pDX, IDC_hDevNames, c_DEVNAMES);
        DDX_Control(pDX, IDC_hDevMode, c_DEVMODE);
        DDX_Control(pDX, IDC_DC_EXTRA, c_DC_EXTRA);
        DDX_Control(pDX, IDC_DC_COPIES, c_DC_COPIES);
        DDX_Control(pDX, IDC_DC_DRIVER, c_DC_DRIVER);
        DDX_Control(pDX, IDC_DC_DUPLEX, c_DC_DUPLEX);
        DDX_Control(pDX, IDC_RESULT, c_Result);
        DDX_Control(pDX, IDC_DC_SIZE, c_DC_SIZE);
        DDX_Control(pDX, IDC_DC_ORIENTATION, c_DC_ORIENTATION);
        DDX_Control(pDX, IDC_DC_MINEXTENT_Y, c_DC_MINEXTENT_Y);
        DDX_Control(pDX, IDC_DC_MINEXTENT_X, c_DC_MINEXTENT_X);
        DDX_Control(pDX, IDC_DC_MAXEXTENT_Y, c_DC_MAXEXTENT_Y);
        DDX_Control(pDX, IDC_DC_MAXEXTENT_X, c_DC_MAXEXTENT_X);
        DDX_Control(pDX, IDC_DC_VERSION, c_DC_VERSION);
        DDX_Control(pDX, IDC_DC_TRUETYPE, c_DC_TRUETYPE);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDeviceCapabilities, CPropertyPage)
        //{{AFX_MSG_MAP(CDeviceCapabilities)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/****************************************************************************
*                               enableControls
* Inputs:
*       BOOL mode: TRUE to enable, FALSE to hide
* Result: void
*       
* Effect: 
*       Hides, shows, enables or disables controls as appropriate
****************************************************************************/

void enableControls(BOOL mode)
    {
     // TODO: implement this
    }

BITCOMBO t_DC_TRUETYPE[] = {
        { DCTT_BITMAP,  IDS_DCTT_BITMAP},
        { DCTT_DOWNLOAD, IDS_DCTT_DOWNLOAD},
        { DCTT_DOWNLOAD_OUTLINE, IDS_DCTT_DOWNLOAD_OUTLINE},
        { DCTT_SUBDEV, IDS_DCTT_SUBDEV},
        { 0, 0} // EOT
                           };

ENUMEDIT t_DC_BINADJUST[] = {
        { DCBA_FACEUPNONE, IDS_DCBA_FACEUPNONE},
        { DCBA_FACEUPCENTER, IDS_DCBA_FACEUPCENTER},
        { DCBA_FACEUPLEFT, IDS_DCBA_FACEUPLEFT},
        { DCBA_FACEUPRIGHT, IDS_DCBA_FACEUPRIGHT},
        { DCBA_FACEDOWNNONE, IDS_DCBA_FACEDOWNNONE},
        { DCBA_FACEDOWNCENTER, IDS_DCBA_FACEDOWNCENTER},
        { DCBA_FACEDOWNLEFT, IDS_DCBA_FACEDOWNLEFT},
        { DCBA_FACEDOWNRIGHT, IDS_DCBA_FACEDOWNRIGHT},
        { 0, 0 } // EOT
                            };

/****************************************************************************
*                   CDeviceCapabilities::setEnumResolutions
* Result: void
*       
* Effect: 
*       Fills in the EnumResolutions dropdown
****************************************************************************/

void CDeviceCapabilities::setEnumResolutions()
    {
     int count = gdc(DC_ENUMRESOLUTIONS, NULL);
     if(count == 0)
        { /* no resolutions */
         c_DC_ENUMRESOLUTIONS.ShowWindow(SW_HIDE);
         c_c_DC_ENUMRESOLUTIONS.EnableWindow(FALSE); 
         return;
        } /* no resolutions */
     c_DC_ENUMRESOLUTIONS.ShowWindow(SW_SHOW);
     c_c_DC_ENUMRESOLUTIONS.EnableWindow(TRUE);

     CPoint * res = new CPoint[count];

     gdc(DC_ENUMRESOLUTIONS, res);
     c_DC_ENUMRESOLUTIONS.ResetContent();

     for(int i = 0; i < count; i++)
        { /* show each */
         CString s;
         s.Format("%d x %d", res[i].x, res[i].y);
         c_DC_ENUMRESOLUTIONS.AddString(s);
        } /* show each */
     delete res;
     c_DC_ENUMRESOLUTIONS.SetCurSel(0);
    }

/****************************************************************************
*                        CDeviceCapabilities::setBins
* Result: void
*       
* Effect: 
*       Loads up the DC_BINNAMES and DC_BINS values
* TODO:
*       Make this owner-draw or use one of the fancier table controls
****************************************************************************/

void CDeviceCapabilities::setBins()
    {
     int count = gdc(DC_BINNAMES, NULL);
     if(count == 0)
        { /* no DC_BINNAMES */
         c_c_DC_BINNAMES.EnableWindow(FALSE);
         c_DC_BINNAMES.ShowWindow(SW_HIDE);
         c_c_DC_BINS.EnableWindow(FALSE);
         c_DC_BINS.ShowWindow(SW_HIDE);
        } /* no DC_BINNAMES */
     else
        { /* has DCBINNAMES */
         typedef struct {char name[24]; } binnames; // Note: specs say 'char'!

         binnames * bn;
         WORD * bnv;

         c_c_DC_BINNAMES.EnableWindow(TRUE);
         c_DC_BINNAMES.ShowWindow(SW_SHOW);
         c_c_DC_BINS.EnableWindow(TRUE);
         c_DC_BINS.ShowWindow(SW_SHOW);

         bn = new binnames[count];
         bnv = new WORD[count];
         
         gdc(DC_BINNAMES, bn);
         gdc(DC_BINS, bnv);
         c_DC_BINNAMES.ResetContent();
         c_DC_BINS.ResetContent();

         for(int i = 0; i < count; i++)
            { /* store names */
             CString s;
             s.Format("%s [%d]", bn[i].name, bnv[i]);
             c_DC_BINNAMES.AddString(s); // AddStringA? 'char'
             s.Format("%3d [%s]", bnv[i], bn[i].name);
             c_DC_BINS.AddString(s);
            } /* store names */

         
         c_DC_BINNAMES.SetCurSel(0);
         c_DC_BINS.SetCurSel(0);
         delete bn;
         delete bnv;
        } /* has DCBINNAMES */
    }

/****************************************************************************
*                  CDeviceCapabilities::setDataTypeProduced
* Result: void
*       
* Effect: 
*       Loads up the DataType_Produced control
****************************************************************************/

void CDeviceCapabilities::setDataTypeProduced()
    {
     int count = gdc(DC_DATATYPE_PRODUCED, NULL);
     
     c_DC_DATATYPE_PRODUCED.ResetContent();

     if(count == -1)
        { /* RAW only */
         CString s;
         s.LoadString(IDS_RAW_ONLY);
         c_DC_DATATYPE_PRODUCED.AddString(s);
         c_DC_DATATYPE_PRODUCED.SetCurSel(0);
        } /* RAW only */
     else
        { /* supports datatype */
         c_DC_DATATYPE_PRODUCED.SetCurSel(0);
        } /* supports datatype */
    }

/****************************************************************************
*                  CDeviceCapabilities::setFileDependencies
* Result: void
*       
* Effect: 
*       Loads file dependencies combo box
****************************************************************************/

void CDeviceCapabilities::setFileDependencies()
    {
     int count = gdc(DC_FILEDEPENDENCIES, NULL);
     if(count == -1)
        { /* no FILEDEPENDENCIES */
         c_DC_FILEDEPENDENCIES.ShowWindow(SW_HIDE);
         c_c_DC_FILEDEPENDENCIES.EnableWindow(FALSE);
        } /* no FILEDEPENDENCIES */
     else
        { /* FILEDEPENDENCIES */
         c_DC_FILEDEPENDENCIES.ShowWindow(SW_SHOW);
         c_c_DC_FILEDEPENDENCIES.EnableWindow(TRUE);
         c_DC_FILEDEPENDENCIES.ResetContent();
         
         typedef struct { char name[64]; } fdnames; // spec says 'char'
         fdnames * fd = new fdnames[count];

         gdc(DC_FILEDEPENDENCIES, fd);
         
         for(int i = 0; i < count; i++)
            { /* load each */
             c_DC_FILEDEPENDENCIES.AddString(fd[i].name);
            } /* load each */

         delete fd;
        } /* FILEDEPENDENCIES */
         
    }

/****************************************************************************
*                     CDeviceCapabilities::setPaperNames
* Result: void
*       
* Effect: 
*       ²
****************************************************************************/

void CDeviceCapabilities::setPaperNames()
    {
     int count = gdc(DC_PAPERNAMES, NULL);

     if(count == 0)
        { /* no paper names */
         c_DC_PAPERNAMES.ShowWindow(SW_HIDE);
         c_c_DC_PAPERNAMES.EnableWindow(FALSE);
        } /* no paper names */

     c_DC_PAPERNAMES.ShowWindow(SW_SHOW);
     c_c_DC_PAPERNAMES.EnableWindow(TRUE);

     typedef struct { char name[64]; } papnames; // spec says 'char'
     papnames * papernames = new papnames[count];
     WORD * papers = new WORD[count];
     POINT * papersizes = new POINT[count];

     gdc(DC_PAPERNAMES, papernames);
     gdc(DC_PAPERS, papers);
     gdc(DC_PAPERSIZE, papersizes);

     c_DC_PAPERNAMES.ResetContent();
     c_DC_PAPERS.ResetContent();
     c_DC_PAPERSIZE.ResetContent();


     CClientDC dc (&c_DC_PAPERNAMES);

     for(int i = 0; i < count; i++)
        { /* list papers */
         PaperInfo * pi = new PaperInfo;
         lstrcpy(pi->papername, papernames[i].name);
         pi->paper = papers[i];
         pi->papersize = papersizes[i];
         c_DC_PAPERNAMES.AddString( pi );
         
         PaperInfo * pi2 = new PaperInfo;
         lstrcpy(pi2->papername, papernames[i].name);
         pi2->paper = papers[i];
         pi2->papersize = papersizes[i];
         c_DC_PAPERS.AddString( pi2 );

         PaperInfo * pi3 = new PaperInfo;
         *pi3 = *pi;
         c_DC_PAPERSIZE.AddString( pi3 );
        } /* list papers */

     c_DC_PAPERNAMES.SetCurSel(0);
     c_DC_PAPERS.SetCurSel(0);
     c_DC_PAPERSIZE.SetCurSel(0);
     delete papernames;
     delete papers;
     delete papersizes;
    }

/****************************************************************************
*                     CDeviceCapabilities::dc_to_controls
* Result: void
*       
* Effect: 
*       Transfers the DeviceCapabilities information to the controls
****************************************************************************/

void CDeviceCapabilities::dc_to_controls()
    {
     if(pd == NULL || pd->hDevNames == NULL || pd->hDevMode == NULL)
        { /* nothing to display */
         enableControls(FALSE);
         return;
        } /* nothing to display */

     _try
        { /* display it */
         dn = (LPDEVNAMES)::GlobalLock(pd->hDevNames);
         if(dn == NULL)
            return;  // TODO: throw exception, set message
         dm = (LPDEVMODE)::GlobalLock(pd->hDevMode);
         if(dn == NULL)
            return;  // TODO: throw exception, set message

         device = (LPCTSTR)&((LPCSTR)dn)[dn->wDeviceOffset];
         port =   (LPCTSTR)&((LPCSTR)dn)[dn->wOutputOffset];

         c_DEVNAMES.SetWindowText(pd->hDevNames);
         c_DEVMODE.SetWindowText(pd->hDevMode);

         int badj = (int)gdc(DC_BINADJUST, NULL);
         if(badj == -1)
            { /* no DC_BINADJUST */
             c_c_DC_BINADJUST.EnableWindow(FALSE);
             c_DC_BINADJUST.ShowWindow(SW_HIDE);
            } /* no DC_BINADJUST */
         else
            { /* has DC_BINADJUST */
             c_c_DC_BINADJUST.EnableWindow(TRUE);
             c_DC_BINADJUST.ShowWindow(SW_SHOW);
             c_DC_BINADJUST.SetWindowText(badj, t_DC_BINADJUST);
            } /* has DC_BINADJUST */

         setBins();

         c_DC_COPIES.SetWindowText(gdc(DC_COPIES, NULL));

         c_DC_DRIVER.SetWindowText(gdc(DC_DRIVER, NULL), "0x%x");

         setDataTypeProduced();
         c_DC_DUPLEX.SetCheck( gdc(DC_DUPLEX, NULL) == 1
                                                ? BST_CHECKED : BST_UNCHECKED);

         c_DC_EMF_COMPLIANT.SetCheck( gdc(DC_EMF_COMPLIANT, NULL) == 1
                                                ? BST_CHECKED : BST_UNCHECKED);

         setEnumResolutions();

         c_DC_EXTRA.SetWindowText(gdc(DC_EXTRA, NULL));

         c_DC_FIELDS.SetWindowText(gdc(DC_FIELDS, NULL), "0x%08x");

         setFileDependencies();

         POINT pt;
         gdc(DC_MAXEXTENT, &pt);
         c_DC_MAXEXTENT_X.SetWindowText(pt.x);
         c_DC_MAXEXTENT_Y.SetWindowText(pt.y);

         gdc(DC_MINEXTENT, &pt);
         c_DC_MINEXTENT_X.SetWindowText(pt.x);
         c_DC_MINEXTENT_Y.SetWindowText(pt.y);

         c_DC_ORIENTATION.SetWindowText(gdc(DC_ORIENTATION, NULL));

         setPaperNames();
         // TODO: DC_PAPERS
         // TODO: DC_PAPERSIZE

         c_DC_SIZE.SetWindowText(gdc(DC_SIZE, NULL));
         c_DC_TRUETYPE.AddBits(gdc(DC_TRUETYPE, NULL), t_DC_TRUETYPE);

         c_DC_VERSION.SetWindowText(gdc(DC_VERSION, NULL), "0x%x");
        } /* display it */
     _finally
        { /* unlock if needed */
         if(dn != NULL)
            ::GlobalUnlock(pd->hDevNames);
         if(dm != NULL)
            ::GlobalUnlock(pd->hDevMode);
        } /* unlock if needed */

    }

/////////////////////////////////////////////////////////////////////////////
// CDeviceCapabilities message handlers

BOOL CDeviceCapabilities::OnKillActive() 
{
        // TODO: Add your specialized code here and/or call the base class
        
        return CPropertyPage::OnKillActive();
}

BOOL CDeviceCapabilities::OnSetActive() 
{
     CWaitCursor wait;
        dc_to_controls();
        
        return CPropertyPage::OnSetActive();
}

BOOL CDeviceCapabilities::OnInitDialog() 
{
     CWaitCursor wait;
        CPropertyPage::OnInitDialog();
        
        // dc_to_controls();
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}
