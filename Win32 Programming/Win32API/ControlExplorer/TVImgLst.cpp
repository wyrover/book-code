// TVImgLst.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "NumericEdit.h"
#include "TinyButt.h"
#include "msglog.h"
#include "ixable.h"
#include "loggable.h"
#include "TreeOps.h"
#include "TVImgLst.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTVImageList property page

IMPLEMENT_DYNCREATE(CTVImageList, CTreeOps)

CTVImageList::CTVImageList() : CTreeOps(CTVImageList::IDD)
{
        //{{AFX_DATA_INIT(CTVImageList)
        //}}AFX_DATA_INIT
}

CTVImageList::~CTVImageList()
{
}

void CTVImageList::DoDataExchange(CDataExchange* pDX)
{
        CTreeOps::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CTVImageList)
        DDX_Control(pDX, IDC_SPINOVERLAYSOURCE, c_SpinOverlaySource);
        DDX_Control(pDX, IDC_SETOVERLAYIMAGE, c_SetOverlayImage);
        DDX_Control(pDX, IDC_OVERLAYSOURCE, c_OverlaySource);
        DDX_Control(pDX, IDC_OVERLAYMASK_CAPTION, c_c_OverlayMask);
        DDX_Control(pDX, IDC_OV4, c_OV4);
        DDX_Control(pDX, IDC_OV3, c_OV3);
        DDX_Control(pDX, IDC_OV2, c_OV2);
        DDX_Control(pDX, IDC_OV1, c_OV1);
        DDX_Control(pDX, IDC_IMAGELISTS, c_ImageLists);
        DDX_Control(pDX, IDC_TVSIL_STATE, c_TVSIL_STATE);
        DDX_Control(pDX, IDC_TVSIL_NORMAL, c_TVSIL_NORMAL);
        DDX_Control(pDX, IDC_SETIMAGELIST, c_SetImageList);
        DDX_Control(pDX, IDC_IMAGES, c_Images);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTVImageList, CTreeOps)
        //{{AFX_MSG_MAP(CTVImageList)
        ON_BN_CLICKED(IDC_SETIMAGELIST, OnSetimagelist)
        ON_CBN_SELENDOK(IDC_IMAGELISTS, OnSelendokImagelists)
        ON_BN_CLICKED(IDC_SETOVERLAYIMAGE, OnSetoverlayimage)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTVImageList message handlers

BOOL CTVImageList::OnInitDialog() 
{
        CTreeOps::OnInitDialog();
        
        CheckRadioButton(IDC_TVSIL_NORMAL, IDC_TVSIL_STATE, IDC_TVSIL_NORMAL);

        CString s;
        s.LoadString(IDS_FULL_IMAGELIST);
        int index = c_ImageLists.AddString(s);
        c_ImageLists.SetItemData(index, (LPARAM)imagelist);

        s.LoadString(IDS_STATE_LIST2);
        index = c_ImageLists.AddString(s);
        c_ImageLists.SetItemData(index, (LPARAM)botlist);

        s.LoadString(IDS_1PIXEL_LIST);
        index = c_ImageLists.AddString(s);
        c_ImageLists.SetItemData(index, (LPARAM)pixel1);

        s.LoadString(IDS_NULL);
        c_ImageLists.AddString(s);

        c_ImageLists.SetCurSel(0);


        LV_COLUMN col;
        col.cx = c_Images.GetStringWidth(_T("0000"));
        col.cx += ::GetSystemMetrics(SM_CXSMICON);
        col.fmt = LVCFMT_RIGHT;
        col.iSubItem = 0;
        col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM;
        c_Images.InsertColumn(0, &col);

        OnSelendokImagelists();

            
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CTVImageList::OnSetimagelist() 
{
 CImageList * img = (CImageList *)c_ImageLists.GetItemData(c_ImageLists.GetCurSel());
 setimage((img == NULL ? NULL : (HIMAGELIST)img->m_hImageList));
}

void CTVImageList::setimage(HIMAGELIST img)
{
 int type;
 CString typestr;
 if(c_TVSIL_STATE.GetCheck() == BST_CHECKED)
    { /* state */
     type = TVSIL_STATE;
     typestr.LoadString(IDS_TVSIL_STATE);
    } /* state */
 else
    { /* normal */
     type = TVSIL_NORMAL;
     typestr.LoadString(IDS_TVSIL_NORMAL);
    } /* normal */

 HIMAGELIST prev = (HIMAGELIST)c_Tree->SendMessage(TVM_SETIMAGELIST, type, 
                                (LPARAM)img);
        
 CString prevstr;
 prevstr.Format(_T("%08x"), (DWORD)prev);

 CString imgname;
 imgname.LoadString(img == NULL ? IDS_NULL : IDS_IMG);
 showResult_s_s_s(IDS_TVM_SETIMAGELIST, typestr, imgname, prevstr);

}

void CTVImageList::OnSelendokImagelists() 
{
     CImageList * imagelist = 
           (CImageList *)c_ImageLists.GetItemData(c_ImageLists.GetCurSel());

     // c_Images.DeleteAllItems();

     if(imagelist == NULL)
        return;


     c_Images.SetImageList(imagelist, LVSIL_SMALL);
     int images = imagelist->GetImageCount();
     
     c_Images.DeleteAllItems();

     for(int i = 0; i < images; i++)
        { /* load images */
         CString s;
         s.Format(_T("%d"), i);

         LV_ITEM item;
         item.iItem = i;
         item.iSubItem = 0;
         item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
         item.pszText = (LPTSTR)(LPCTSTR)s;
         item.iImage = i;
         item.lParam = i;
         c_Images.InsertItem(&item);
        } /* load images */
        
     enableControls();
}

void CTVImageList::enableControls()
{
 CImageList * imagelist = (CImageList *)c_ImageLists.GetItemData(c_ImageLists.GetCurSel());
 BOOL enable = (imagelist != NULL);

 c_SpinOverlaySource.EnableWindow(enable);
 c_SetOverlayImage.EnableWindow(enable);
 c_OverlaySource.EnableWindow(enable);
 c_c_OverlayMask.EnableWindow(enable);
 c_OV1.EnableWindow(enable);
 c_OV2.EnableWindow(enable);
 c_OV3.EnableWindow(enable);
 c_OV4.EnableWindow(enable);
 if(enable)
    { /* set spin range */
     c_SpinOverlaySource.SetRange(0, imagelist->GetImageCount() - 1);
    } /* set spin range */
}


void CTVImageList::OnSetoverlayimage() 
{
 // Retrieve the current image list and set the specified mask
 CImageList * imagelist = (CImageList *)c_ImageLists.GetItemData(c_ImageLists.GetCurSel());
 if(imagelist == NULL)
    return;  // should never occur, but just in case...

 int ov = 0;
 if(c_OV1.GetCheck() == BST_CHECKED)
    ov = 1;
 else
 if(c_OV2.GetCheck() == BST_CHECKED)
    ov = 2;
 else
 if(c_OV3.GetCheck() == BST_CHECKED)
    ov = 3;
 else
 if(c_OV4.GetCheck() == BST_CHECKED)
    ov = 4;

 int source = c_OverlaySource.GetWindowInt();
 int result = imagelist->SetOverlayImage(source, ov);
 showResult_d_d_d(IDS_SETOVERLAYIMAGE, source, ov, result);
}
