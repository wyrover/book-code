// HdrMsg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "ixable.h"
#include "loggable.h"
#include "NumericEdit.h"
#include "TinyButt.h"
#include "image.h"
#include "bmselect.h"
#include "HdrMsg.h"
#include "bitmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DIM(x) (sizeof(x) / sizeof( (x)[0]))

static int bitmaps[] = {
        IDB_BITMAP1,
        IDB_BITMAP7,
        IDB_BITMAP2,
        IDB_BITMAP3,
        IDB_BITMAP4,
        IDB_BITMAP5,
        IDB_BITMAP6,
        0       // EOT
                };

/////////////////////////////////////////////////////////////////////////////
// CHeaderMsg property page

IMPLEMENT_DYNCREATE(CHeaderMsg, CLoggingPage)

CHeaderMsg::CHeaderMsg() : CLoggingPage(CHeaderMsg::IDD)
{
 memset(&hdl, 0, sizeof(hdl));

        //{{AFX_DATA_INIT(CHeaderMsg)
        //}}AFX_DATA_INIT
}

CHeaderMsg::~CHeaderMsg()
{
}

void CHeaderMsg::DoDataExchange(CDataExchange* pDX)
{
        CLoggingPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CHeaderMsg)
        DDX_Control(pDX, IDC_PLUS12, c_Plus12);
        DDX_Control(pDX, IDC_COMPUTEWIDTH, c_ComputeWidth);
        DDX_Control(pDX, IDC_ITEMCOUNT, c_ItemCount);
        DDX_Control(pDX, IDC_LPARAM_CAPTION, c_c_lParam);
        DDX_Control(pDX, IDC_LPARAM, c_lParam);
        DDX_Control(pDX, IDC_TEXT, c_Text);
        DDX_Control(pDX, IDC_TEXT_CAPTION, c_c_Text);
        DDX_Control(pDX, IDC_SPINITEM, c_SpinItem);
        DDX_Control(pDX, IDC_ITEM, c_Item);
        DDX_Control(pDX, IDC_HDM_SETITEM, c_HDM_SETITEM);
        DDX_Control(pDX, IDC_HDM_INSERTITEM, c_HDM_INSERTITEM);
        DDX_Control(pDX, IDC_HDM_GETITEM, c_HDM_GETITEM);
        DDX_Control(pDX, IDC_HDM_DELETEITEM, c_HDM_DELETEITEM);
        DDX_Control(pDX, IDC_HDI_WIDTH, c_HDI_WIDTH);
        DDX_Control(pDX, IDC_HDI_TEXT, c_HDI_TEXT);
        DDX_Control(pDX, IDC_HDI_LPARAM, c_HDI_LPARAM);
        DDX_Control(pDX, IDC_HDI_HEIGHT, c_HDI_HEIGHT);
        DDX_Control(pDX, IDC_HDI_FORMAT, c_HDI_FORMAT);
        DDX_Control(pDX, IDC_HDI_BITMAP, c_HDI_BITMAP);
        DDX_Control(pDX, IDC_HDF_STRING, c_HDF_STRING);
        DDX_Control(pDX, IDC_HDF_RTLREADING, c_HDF_RTLREADING);
        DDX_Control(pDX, IDC_HDF_RIGHT, c_HDF_RIGHT);
        DDX_Control(pDX, IDC_HDF_OWNERDRAW, c_HDF_OWNERDRAW);
        DDX_Control(pDX, IDC_HDF_LEFT, c_HDF_LEFT);
        DDX_Control(pDX, IDC_HDF_CENTER, c_HDF_CENTER);
        DDX_Control(pDX, IDC_HDF_BITMAP, c_HDF_BITMAP);
        DDX_Control(pDX, IDC_HBM_CAPTION, c_c_hbm);
        DDX_Control(pDX, IDC_HBM, c_hbm);
        DDX_Control(pDX, IDC_FMT_CAPTION, c_c_fmt);
        DDX_Control(pDX, IDC_CXY_CAPTION, c_c_cxy);
        DDX_Control(pDX, IDC_CXY, c_cxy);
        DDX_Control(pDX, IDC_CCHTEXTMAX, c_cchTextMax);
        DDX_Control(pDX, IDC_CCHMAX_CAPTION, c_c_cchTextMax);
        //}}AFX_DATA_MAP
#ifdef HBT_SPRING
        DDX_Control(pDX, IDC_SPRINGY, c_Springy);
#endif
}


BEGIN_MESSAGE_MAP(CHeaderMsg, CLoggingPage)
        //{{AFX_MSG_MAP(CHeaderMsg)
        ON_BN_CLICKED(IDC_HDI_BITMAP, OnHdiBitmap)
        ON_BN_CLICKED(IDC_HDI_FORMAT, OnHdiFormat)
        ON_BN_CLICKED(IDC_HDI_HEIGHT, OnHdiHeight)
        ON_BN_CLICKED(IDC_HDI_LPARAM, OnHdiLparam)
        ON_BN_CLICKED(IDC_HDI_TEXT, OnHdiText)
        ON_BN_CLICKED(IDC_HDI_WIDTH, OnHdiWidth)
        ON_BN_CLICKED(IDC_HDM_GETITEMCOUNT, OnHdmGetitemcount)
        ON_BN_CLICKED(IDC_HDM_DELETEITEM, OnHdmDeleteitem)
        ON_BN_CLICKED(IDC_HDM_GETITEM, OnHdmGetitem)
        ON_BN_CLICKED(IDC_HDM_INSERTITEM, OnHdmInsertitem)
        ON_BN_CLICKED(IDC_HDM_SETITEM, OnHdmSetitem)
        ON_BN_CLICKED(IDC_COMPUTEWIDTH, OnComputewidth)
        ON_BN_CLICKED(IDC_SETFONT, OnSetfont)
        ON_BN_CLICKED(IDC_PLUS12, OnPlus12)
        //}}AFX_MSG_MAP
#ifdef HBT_SPRING
        ON_BN_CLICKED(IDC_SPRINGY, OnSpringy)
#endif
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeaderMsg message handlers

BOOL CHeaderMsg::OnInitDialog() 
{
        CLoggingPage::OnInitDialog();
        
        c_hbm.Load(bitmaps, -1);

        c_SpinItem.SetRange(0, 100);

#ifdef HBT_SPRING
        c_Springy.ShowWindow(SW_SHOW);
#endif  

        enableControls();
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

/****************************************************************************
*                             CHeaderMsg::addMessage
* Inputs:
*       CString s: Contents of message text 
* Result: void
*       
* Effect: 
*       Logs the message including the window id and type.  The string s
*       has been formatted by a showResult call.
****************************************************************************/

void CHeaderMsg::addMessage(CString s)
    {
     CString ctlName;
     ctlName.LoadString(IDS_HEADER);

     c_Messages->addMessage(c_Header->m_hWnd, ctlName, s);
    }


/****************************************************************************
*                         CHeaderMsg::enableControls
* Result: void
*       
* Effect: 
*       Enables controls based on the internal state
****************************************************************************/

#define OWNERDRAW_IMPLEMENTED FALSE  // NYI


void CHeaderMsg::enableControls()
    {
     BOOL enable;

     enable = c_HDI_TEXT.GetCheck();
     c_Text.EnableWindow(enable);
     c_c_Text.EnableWindow(enable);

     enable = c_HDI_TEXT.GetCheck() || c_HDI_BITMAP.GetCheck();
     c_ComputeWidth.EnableWindow(enable);
     c_Plus12.EnableWindow(enable);

     enable = c_HDI_WIDTH.GetCheck();
     enable |= c_HDI_HEIGHT.GetCheck();
     c_cxy.EnableWindow(enable);
     c_c_cxy.EnableWindow(enable);

     enable = c_HDI_FORMAT.GetCheck();
     c_HDF_STRING.EnableWindow(enable);
     c_HDF_RTLREADING.EnableWindow(enable);
     c_HDF_OWNERDRAW.EnableWindow(enable && OWNERDRAW_IMPLEMENTED);
     c_HDF_BITMAP.EnableWindow(enable);

     c_HDF_LEFT.EnableWindow(enable);
     c_HDF_CENTER.EnableWindow(enable);
     c_HDF_RIGHT.EnableWindow(enable);
     c_c_fmt.EnableWindow(enable);

     enable = c_HDI_LPARAM.GetCheck();
     c_lParam.EnableWindow(enable);
     c_c_lParam.EnableWindow(enable);

     enable = c_HDI_BITMAP.GetCheck();
     c_hbm.EnableWindow(enable);
     c_c_hbm.EnableWindow(enable);

    }


/****************************************************************************
*                          CHeaderMsg::hdlToControls
* Result: void
*       
* Effect: 
*       Takes the contents of the hdl structure and sets the controls to
*       match them
****************************************************************************/

void CHeaderMsg::hdlToControls()
    {

     //================================================================
     // Set the bitmap value
     //================================================================

     if(hdl.mask & HDI_BITMAP)
        { /* select bitmap */
         // NYI
        } /* select bitmap */

     //================================================================
     // Set the fmt values
     //================================================================

     int justify = 0;
     int type = 0;

     if(hdl.mask & HDI_FORMAT)
        { /* select format */
         if((hdl.fmt & HDF_JUSTIFYMASK) == HDF_CENTER)
            justify = IDC_HDF_CENTER;
         else
         if((hdl.fmt & HDF_JUSTIFYMASK) == HDF_LEFT)
            justify = IDC_HDF_LEFT;
         else
         if((hdl.fmt & HDF_JUSTIFYMASK) == HDF_RIGHT)
            justify = IDC_HDF_RIGHT;
         else
         if(hdl.fmt & HDF_RTLREADING)
            justify = IDC_HDF_RTLREADING;

         if(hdl.fmt & HDF_BITMAP)
            type = IDC_HDF_BITMAP;
         else
         if(hdl.fmt & HDF_OWNERDRAW)
            type = IDC_HDF_OWNERDRAW;
         else
         if(hdl.fmt & HDF_STRING)
            type = IDC_HDF_STRING;
        } /* select format */
     
     CheckRadioButton(IDC_HDF_CENTER, IDC_HDF_RTLREADING, justify);
     CheckRadioButton(IDC_HDF_BITMAP, IDC_HDF_STRING, type);

     //================================================================
     // Set the cxy values
     //================================================================
     if(hdl.mask & (HDI_WIDTH | HDI_HEIGHT))
        c_cxy.SetWindowText(hdl.cxy);

     //================================================================
     // Set the lParam fields
     //================================================================

     if(hdl.mask & HDI_LPARAM)
         c_lParam.SetWindowText(hdl.lParam);

     //================================================================
     // Set the text fields
     //================================================================

     if(hdl.mask & HDI_TEXT)
        { /* text available */
         c_Text.SetWindowText(hdl.pszText);
         c_cchTextMax.SetWindowText(hdl.cchTextMax);
        } /* text available */

    }

/****************************************************************************
*                             CHeaderMsg::getMask
* Result: UINT
*       The mask value as derived from the controls
****************************************************************************/

UINT CHeaderMsg::getMask()
    {
     UINT mask = 0;

     if(c_HDI_BITMAP.GetCheck())
        mask |= HDI_BITMAP;

     if(c_HDI_FORMAT.GetCheck())
        mask |= HDI_FORMAT;
 
     if(c_HDI_HEIGHT.GetCheck())
        mask |= HDI_HEIGHT;

     if(c_HDI_LPARAM.GetCheck())
        mask |= HDI_LPARAM;

     if(c_HDI_TEXT.GetCheck())
        mask |= HDI_TEXT;

     if(c_HDI_WIDTH.GetCheck())
        mask |= HDI_WIDTH;

     return mask;
    }

/****************************************************************************
*                             CHeaderMsg::getFmt
* Result: int
*       Flag values for format, obtained from the controls
****************************************************************************/

int CHeaderMsg::getFmt()
    {
     int flags = 0;

     if(c_HDF_CENTER.GetCheck())
        flags |= HDF_CENTER;

     if(c_HDF_LEFT.GetCheck())
        flags |= HDF_LEFT;

     if(c_HDF_RIGHT.GetCheck())
        flags |= HDF_RIGHT;

     if(c_HDF_RTLREADING.GetCheck())
        flags |= HDF_RTLREADING;

     if(c_HDF_BITMAP.GetCheck())
        flags |= HDF_BITMAP;

     if(c_HDF_OWNERDRAW.GetCheck())
        flags |= HDF_OWNERDRAW;

     if(c_HDF_STRING.GetCheck())
        flags |= HDF_STRING;

     return flags;
    }

/****************************************************************************
*                          CHeaderMsg::controlsToHdl
* Result: void
*       
* Effect: 
*       Takes the values in the controls and copies them to the hdl structure
****************************************************************************/

void CHeaderMsg::controlsToHdl()
    {
     hdl.mask = getMask();
     hdl.cxy  = c_cxy.GetWindowInt();

     if(hdl.mask & HDI_TEXT)
        { /* has text */
         c_Text.GetWindowText(textarea);
         hdl.pszText = (LPTSTR)(LPCTSTR)textarea;
        } /* has text */
     else
        { /* no text */
         hdl.pszText = NULL;
        } /* no text */

     hdl.cchTextMax = 0;  // note that for GetItem we fix this in the
                          // OnGetItem handler

     hdl.fmt = getFmt();

     if(hdl.mask & HDI_BITMAP)
        { /* bitmap */
         int index = c_hbm.GetCurSel();
         if(index == CB_ERR)
            { /* no bitmap */
             desired_bitmap = 0;
            } /* no bitmap */
         else
            { /* load bitmap */
             desired_bitmap = c_hbm.GetItemData(index);
            } /* load bitmap */
        } /* bitmap */
    }

void CHeaderMsg::OnHdiBitmap() 
{
 enableControls();      
}

void CHeaderMsg::OnHdiFormat() 
{
 enableControls();      
}

void CHeaderMsg::OnHdiHeight() 
{
 if(c_HDI_HEIGHT.GetCheck())
    c_HDI_WIDTH.SetCheck(BST_UNCHECKED);

 enableControls();      
}

void CHeaderMsg::OnHdiLparam() 
{
 enableControls();
}

void CHeaderMsg::OnHdiText() 
{
 enableControls();
}

void CHeaderMsg::OnHdiWidth() 
{
 if(c_HDI_WIDTH.GetCheck())
    c_HDI_HEIGHT.SetCheck(BST_UNCHECKED);

 enableControls();
}

void CHeaderMsg::OnHdmGetitemcount() 
{
 int result = ::SendMessage(c_Header->m_hWnd, HDM_GETITEMCOUNT, 0, 0);
 showResult_d_d_d(IDS_HDM_GETITEMCOUNT, 0, 0, result);
 c_ItemCount.SetWindowText(result);
 hdlToControls();
}

void CHeaderMsg::OnHdmDeleteitem() 
{
 int item = c_Item.GetWindowInt();
 int result = ::SendMessage(c_Header->m_hWnd, HDM_GETITEMCOUNT, item, 0);
 showResult_d_d_d(IDS_HDM_DELETEITEM, item, 0, result);

 // We could update the item count here, but currently choose not to.
}

/****************************************************************************
*                         CHeaderMsg::preDeleteOldBitmap
* Result: void
*       
* Effect: 
*       If the current hdl is going to set a bitmap, we first retrieve the
*       existing bitmap and delete it, otherwise we get dangling bitmaps.
*
* Notes:
*       We can't actually delete the bitmap until we have successfully 
*       replaced it
****************************************************************************/

void CHeaderMsg::preDeleteOldBitmap()
    {
     if((hdl.mask & HDI_BITMAP) && (hdl.fmt & HDF_BITMAP))
        { /* replacing bitmap */
         HD_ITEM thdl = hdl;

         thdl.mask |= HDI_BITMAP;  // retrieve bitmap unconditionally

         int item = c_Item.GetWindowInt();
         int result = ::SendMessage(c_Header->m_hWnd, HDM_GETITEM, item,
                                    (LPARAM)&thdl);

         if(result >= 0 && thdl.hbm != 0)
            { /* has old BM */
             deletion_candidate = thdl.hbm;
            } /* has old BM */
         else
            { /* no old BM */
             deletion_candidate = 0;
            } /* no old BM */
        } /* replacing bitmap */
    }

/****************************************************************************
*                         CHeaderMsg::forceNewBitmap
* Result: void
*       
* Effect: 
*       Loads the new bitmap, if one is needed
****************************************************************************/

void CHeaderMsg::forceNewBitmap()
    {
     if((hdl.mask & HDI_BITMAP) && (hdl.fmt & HDF_BITMAP))
        { /* wants bitmap */
         if(desired_bitmap != 0)
           { /* load it */
            CBitmap bi;
            bi.LoadBitmap(desired_bitmap);
            hdl.hbm = (HBITMAP)bi;
            bi.Detach();
           } /* load it */
        else
           { /* no bitmap */
            hdl.hbm = 0;
           } /* no bitmap */
        } /* wants bitmap */
    }

/****************************************************************************
*                       CHeaderMsg::postDeleteOldBitmap
* Result: void
*       
* Effect: 
*       Completes the deletion of a bitmap that has been replaced
****************************************************************************/

void CHeaderMsg::postDeleteOldBitmap()
    {
     if(deletion_candidate != NULL)
        { /* delete it */
         ::DeleteObject(deletion_candidate);
         deletion_candidate = NULL;
        } /* delete it */
    }

/****************************************************************************
*                      CHeaderMsg::selectMatchingBitmap
* Result: void
*       
* Effect: 
*       Locates the bitmap that matches the current bitmap and selects it
* Notes:
*       This is a fairly grotesque algorithm...it actually compares the
*       bits of the bitmaps!  This is because for the Explorer we have no
*       really good way of storing the bitmap information to use on
*       retrieval.  It is only the artifice of the c_hbm bitmap selector
*       dropdown that necessitates this peculiar hack.
****************************************************************************/

void CHeaderMsg::selectMatchingBitmap()
    {

     c_hbm.SetCurSel(CB_ERR); // Assume no match to start...

     if(hdl.hbm == NULL)
         return; // all done...

     CBitmapData b1(hdl.hbm);

     for(int i = 0; i < c_hbm.GetCount(); i++)
        { /* search it */
         int id = (int)c_hbm.GetItemData(i);
         HBITMAP b2bm = ::LoadBitmap(AfxGetInstanceHandle(), 
                                                MAKEINTRESOURCE(id));
         CBitmapData b2(b2bm);

         if(b1 == b2)
            { /* found it */
             c_hbm.SetCurSel(i);
             ::DeleteObject(b2bm);
             break;
            } /* found it */
         ::DeleteObject(b2bm);
        } /* search it */
     
    }

void CHeaderMsg::OnHdmGetitem() 
{
 TCHAR text[256];
 controlsToHdl();

 // Now fix up the buffer values for the string receipt

 if(hdl.mask & HDI_TEXT)
    { /* fix up text */
     hdl.pszText = text;
     hdl.cchTextMax = DIM(text);
    } /* fix up text */

 int item = c_Item.GetWindowInt();
 int result = ::SendMessage(c_Header->m_hWnd, HDM_GETITEM, item, (LPARAM)&hdl);
 showResult_d_s_d(IDS_HDM_GETITEM, item, _T("&hdl"), result);

 // Retrieve the bitmap ID we squirelled away (this is not normal, but
 // is a requirement of the way we want to handle combo box selection
 // in the explorer)

 selectMatchingBitmap();

 hdlToControls();
}

void CHeaderMsg::OnHdmInsertitem() 
{
 controlsToHdl();

 forceNewBitmap();

 int item = c_Item.GetWindowInt();
 int result = ::SendMessage(c_Header->m_hWnd, HDM_INSERTITEM, item, (LPARAM)&hdl);
 showResult_d_s_d(IDS_HDM_INSERTITEM, item, _T("&hdl"), result);

 hdlToControls();
}

void CHeaderMsg::OnHdmSetitem() 
{
 controlsToHdl();

 preDeleteOldBitmap();
 forceNewBitmap();

 int item = c_Item.GetWindowInt();
 int result = ::SendMessage(c_Header->m_hWnd, HDM_SETITEM, item, (LPARAM)&hdl);

 // Save the bitmap ID for later retrieval.  We need to save this because
 // we make a copy of each bitmap for each usage, so there is no unique
 // representation

 showResult_d_s_d(IDS_HDM_SETITEM, item, _T("&hdl"), result);
 if(result >= 0)
    { /* success */
     postDeleteOldBitmap();
    } /* success */
}

/****************************************************************************
*                          CHeaderMsg::computeWidth
* Inputs:
*       int bias: Bias value
* Result: void
*       
* Effect: 
*       Computes the width from the string, plus the bias value.
****************************************************************************/

void CHeaderMsg::computeWidth(int bias)
    {
     int width = 0;

     controlsToHdl();

     if(hdl.mask & HDI_TEXT)
        { /* has text */
         CFont * font = c_Header->GetFont();
         CClientDC dc(c_Header);

         dc.SelectObject(font);

         SIZE sz;
         CString s;
         c_Text.GetWindowText(s);

         sz = dc.GetTextExtent(s);

         width = sz.cx;
        } /* has text */

     if(hdl.mask & HDI_BITMAP)
        { /* has bitmap */
         int index = c_hbm.GetCurSel();
         if(index != CB_ERR)
            { /* bitmap selected */
             BITMAPINFO bmi;
             CBitmap bi;
             CClientDC dc(c_Header);

             bi.m_hObject = ::LoadBitmap(AfxGetInstanceHandle(), 
                                    MAKEINTRESOURCE(c_hbm.GetItemData(index)));
             bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
             bmi.bmiHeader.biBitCount = 0;
             ::GetDIBits(dc.m_hDC, bi, 0, 0, NULL, &bmi, DIB_RGB_COLORS);
             bi.DeleteObject();
             width += bmi.bmiHeader.biWidth;
            } /* bitmap selected */
        } /* has bitmap */

     if(width > 0)
        { /* has width */
         width += bias;
         c_cxy.SetWindowText(width);

         c_HDI_WIDTH.SetCheck(BST_CHECKED);
         c_HDI_HEIGHT.SetCheck(BST_UNCHECKED);
        } /* has width */
     enableControls();
     
    }

void CHeaderMsg::OnComputewidth() 
{
    computeWidth(0);    
}

void CHeaderMsg::OnSetfont() 
{
 DWORD flags = CF_SCREENFONTS;
 CFont * font = c_Header->GetFont();
 LOGFONT lf;

 if(font != NULL)
    { /* has font */
     flags |= CF_INITTOLOGFONTSTRUCT;
     font->GetObject(sizeof(lf), &lf);
    } /* has font */
 else
    { /* no font */
     memset(&lf, 0, sizeof(lf));
    } /* no font */


 CFontDialog dlg(&lf, flags);
 if(dlg.DoModal() == IDOK)
    { /* set font */
     CFont newfont;
     if(newfont.CreateFontIndirect(&lf))
        { /* success */
         c_Header->SetFont(&newfont);
         if(font != NULL)
            font->DeleteObject();
         newfont.Detach();  // so destructor doesn't destroy font
        } /* success */
    } /* set font */

}

void CHeaderMsg::OnPlus12() 
{
    computeWidth(12);
}

#ifdef HBT_SPRING
void CHeaderMsg::OnSpringy() 
{
 int item = c_Item.GetWindowInt();
 result = ::SendMessage(c_Header->m_hWnd, WM_SETTEXT, HBT_SPRING | item, 0);

 CString ctlName;
 ctlName.LoadString(IDS_HEADER);

 CString fmt;
 fmt.LoadString(IDS_SPRINGY);
 // WM_SETTEXT(0x%08x=HBT_SPRING|%d, 0)
 
 CString s;
 s.Format(fmt, HBT_SPRING | item, item);

 c_Messages->addMessage(c_Header->m_hWnd, ctlName, s);
}
#endif
