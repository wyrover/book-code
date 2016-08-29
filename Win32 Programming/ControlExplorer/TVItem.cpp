#include "stdafx.h"
#include "ControlExplorer.h"

#include "NumericEdit.h"
#include "msglog.h"
#include "ixable.h"
#include "loggable.h"
#include "treeops.h"
#include "tvitem.h"

#define DIM(x) (sizeof(x) / sizeof((x)[0]))

#define OVERLAYMASKTOINDEX(x) ( ((x) & TVIS_OVERLAYMASK) >> 8)
#define STATEIMAGEMASKTOINDEX(x) ( ((x) & TVIS_STATEIMAGEMASK) >> 12)

IMPLEMENT_DYNCREATE(CTV_ITEM, CTreeOps)

TV_ITEM CTV_ITEM::item;
TCHAR CTV_ITEM::text[256];

void CTV_ITEM::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CTV_ITEM)
        DDX_Control(pDX, IDC_CCHILDREN, c_cChildren);
        DDX_Control(pDX, IDC_CCHILDREN_CAPTION, c_c_cChildren);
        DDX_Control(pDX, IDC_CHECKALL, c_CheckAll);
        DDX_Control(pDX, IDC_HITEM, c_hItem);
        DDX_Control(pDX, IDC_HITEM_CAPTION, c_c_hItem);
        DDX_Control(pDX, IDC_IIMAGE, c_iImage);
        DDX_Control(pDX, IDC_IIMAGE_CALLBACK, c_iImage_Callback);
        DDX_Control(pDX, IDC_IIMAGE_CAPTION, c_c_iImage);
        DDX_Control(pDX, IDC_ISELECTEDIMAGE, c_iSelectedImage);
        DDX_Control(pDX, IDC_ISELECTEDIMAGE_CALLBACK, c_iSelectedImage_Callback);
        DDX_Control(pDX, IDC_ISELECTEDIMAGE_CAPTION, c_c_iSelectedImage);
        DDX_Control(pDX, IDC_LPARAM, c_lParam);
        DDX_Control(pDX, IDC_MASK_TVIS_BOLD, c_m_TVIS_BOLD);
        DDX_Control(pDX, IDC_MASK_TVIS_CUT, c_m_TVIS_CUT);
        DDX_Control(pDX, IDC_MASK_TVIS_DROPHILITED, c_m_TVIS_DROPHILITED);
        DDX_Control(pDX, IDC_MASK_TVIS_EXPANDED, c_m_TVIS_EXPANDED);
        DDX_Control(pDX, IDC_MASK_TVIS_EXPANDEDONCE, c_m_TVIS_EXPANDEDONCE);
        //DDX_Control(pDX, IDC_MASK_TVIS_FOCUSED, c_m_TVIS_FOCUSED);
        DDX_Control(pDX, IDC_MASK_TVIS_OVERLAYMASK, c_m_TVIS_OVERLAYMASK);
        DDX_Control(pDX, IDC_MASK_TVIS_SELECTED, c_m_TVIS_SELECTED);
        DDX_Control(pDX, IDC_MASK_TVIS_STATEIMAGEMASK, c_m_TVIS_STATEIMAGEMASK);
        DDX_Control(pDX, IDC_OVERLAY, c_Overlay);
        DDX_Control(pDX, IDC_PARAM_CAPTION, c_c_lParam);
        DDX_Control(pDX, IDC_SPINIIMAGE, c_Spin_iImage);
        DDX_Control(pDX, IDC_SPINISELECTEDIMAGE, c_Spin_iSelectedImage);
        DDX_Control(pDX, IDC_SPINLPARAM, c_Spin_lParam);
        DDX_Control(pDX, IDC_SPINOVERLAY, c_SpinOverlay);
        DDX_Control(pDX, IDC_SPINSTATEIMAGE, c_SpinStateImage);
        DDX_Control(pDX, IDC_STATEIMAGE, c_StateImage);
        DDX_Control(pDX, IDC_STATEMASK_CAPTION, c_c_stateMask);
        DDX_Control(pDX, IDC_STATE_CAPTION, c_c_state);
        DDX_Control(pDX, IDC_STATE_STATEMASK_CAPTION, c_c_State_StateMask);
        DDX_Control(pDX, IDC_TEXT, c_pszText);
        DDX_Control(pDX, IDC_TEXT_CALLBACK, c_pszText_Callback);
        DDX_Control(pDX, IDC_TEXT_CAPTION, c_c_pszText);
        DDX_Control(pDX, IDC_TVIF_CHILDREN, c_TVIF_CHILDREN);
        DDX_Control(pDX, IDC_TVIF_HANDLE, c_TVIF_HANDLE);
        DDX_Control(pDX, IDC_TVIF_IMAGE, c_TVIF_IMAGE);
        DDX_Control(pDX, IDC_TVIF_PARAM, c_TVIF_PARAM);
        DDX_Control(pDX, IDC_TVIF_SELECTEDIMAGE, c_TVIF_SELECTEDIMAGE);
        DDX_Control(pDX, IDC_TVIF_STATE, c_TVIF_STATE);
        DDX_Control(pDX, IDC_TVIF_TEXT, c_TVIF_TEXT);
        DDX_Control(pDX, IDC_TVIS_BOLD, c_TVIS_BOLD);
        DDX_Control(pDX, IDC_TVIS_CUT, c_TVIS_CUT);
        DDX_Control(pDX, IDC_TVIS_DROPHILITED, c_TVIS_DROPHILITED);
        DDX_Control(pDX, IDC_TVIS_EXPANDED, c_TVIS_EXPANDED);
        DDX_Control(pDX, IDC_TVIS_EXPANDEDONCE, c_TVIS_EXPANDEDONCE);
        //DDX_Control(pDX, IDC_TVIS_FOCUSED, c_TVIS_FOCUSED);
        DDX_Control(pDX, IDC_TVIS_OVERLAYMASK, c_TVIS_OVERLAYMASK);
        DDX_Control(pDX, IDC_TVIS_SELECTED, c_TVIS_SELECTED);
        DDX_Control(pDX, IDC_TVIS_STATEIMAGEMASK, c_TVIS_STATEIMAGEMASK);
        //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTV_ITEM, CTreeOps)
       //{{AFX_MSG_MAP(CTV_ITEM)
        ON_BN_CLICKED(IDC_CHECKALL, OnCheckall)
        ON_BN_CLICKED(IDC_TEXT_CALLBACK, OnTextCallback)
        ON_BN_CLICKED(IDC_IIMAGE_CALLBACK, OnIimageCallback)
        ON_BN_CLICKED(IDC_ISELECTEDIMAGE_CALLBACK, OnIselectedimageCallback)
        ON_BN_CLICKED(IDC_MASK_TVIS_BOLD, OnMaskTvisBold)
        ON_BN_CLICKED(IDC_MASK_TVIS_CUT, OnMaskTvisCut)
        ON_BN_CLICKED(IDC_MASK_TVIS_DROPHILITED, OnMaskTvisDrophilited)
        ON_BN_CLICKED(IDC_MASK_TVIS_EXPANDED, OnMaskTvisExpanded)
        ON_BN_CLICKED(IDC_MASK_TVIS_EXPANDEDONCE, OnMaskTvisExpandedonce)
        //ON_BN_CLICKED(IDC_MASK_TVIS_FOCUSED, OnMaskTvisFocused)
        ON_BN_CLICKED(IDC_MASK_TVIS_OVERLAYMASK, OnMaskTvisOverlaymask)
        ON_BN_CLICKED(IDC_MASK_TVIS_SELECTED, OnMaskTvisSelected)
        ON_BN_CLICKED(IDC_MASK_TVIS_STATEIMAGEMASK, OnMaskTvisStateimagemask)
        ON_BN_CLICKED(IDC_TVIF_CHILDREN, OnTvifChildren)
        ON_BN_CLICKED(IDC_TVIF_HANDLE, OnTvifHandle)
        ON_BN_CLICKED(IDC_TVIF_IMAGE, OnTvifImage)
        ON_BN_CLICKED(IDC_TVIF_PARAM, OnTvifParam)
        ON_BN_CLICKED(IDC_TVIF_SELECTEDIMAGE, OnTvifSelectedimage)
        ON_BN_CLICKED(IDC_TVIF_STATE, OnTvifState)
        ON_BN_CLICKED(IDC_TVIF_TEXT, OnTvifText)
       //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CTV_ITEM::CTV_ITEM() : CTreeOps((int)0) // should never call this
{
 ASSERT(FALSE); // Must never call in context where default constructor used
 item.pszText = text;
 item.cchTextMax = DIM(text);
}

CTV_ITEM::CTV_ITEM(int idd) : CTreeOps(idd)
{
        //{{AFX_DATA_INIT(CTree)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CTV_ITEM::~CTV_ITEM()
{
}


BOOL CTV_ITEM::OnInitDialog()
{
 CTreeOps::OnInitDialog();

 c_cChildren.ResetContent();
 CString s;
 s.LoadString(IDS_CHILD_0);
 c_cChildren.AddString(s);

 s.LoadString(IDS_CHILD_1);
 int index = c_cChildren.AddString(s);
 c_cChildren.SetItemData(index, 1);

 s.LoadString(IDS_CHILD_CALLBACK);
 index = c_cChildren.AddString(s);
 c_cChildren.SetItemData(index, (DWORD)I_CHILDRENCALLBACK);

 c_cChildren.SetCurSel(CB_ERR);  // no selection

 c_SpinOverlay.SetRange(0,4);
 c_SpinStateImage.SetRange(0,15);

 return TRUE;
}

/****************************************************************************
*                          CTV_ITEM::enableControls
* Result: void
*       
* Effect: 
*       Selectively enables controls for the TV_ITEM objects in the dialog
****************************************************************************/

void CTV_ITEM::enableControls()
    {
     BOOL enable = c_TVIF_STATE.GetCheck() == BST_CHECKED;

     c_c_State_StateMask.EnableWindow(enable);
     c_c_state.EnableWindow(enable);
     c_c_stateMask.EnableWindow(enable);

     c_m_TVIS_BOLD.EnableWindow(enable);
     c_m_TVIS_CUT.EnableWindow(enable);
     c_m_TVIS_DROPHILITED.EnableWindow(enable);
     c_m_TVIS_EXPANDED.EnableWindow(enable);
     c_m_TVIS_EXPANDEDONCE.EnableWindow(enable);
     //c_m_TVIS_FOCUSED.EnableWindow(enable);
     c_m_TVIS_OVERLAYMASK.EnableWindow(enable);
     c_m_TVIS_SELECTED.EnableWindow(enable);
     c_m_TVIS_STATEIMAGEMASK.EnableWindow(enable);
     c_CheckAll.EnableWindow(enable);

     c_TVIS_BOLD.EnableWindow(enable && c_m_TVIS_BOLD.GetCheck());
     c_TVIS_CUT.EnableWindow(enable && c_m_TVIS_CUT.GetCheck());
     c_TVIS_DROPHILITED.EnableWindow(enable && c_m_TVIS_DROPHILITED.GetCheck());
     c_TVIS_EXPANDED.EnableWindow(enable && c_m_TVIS_EXPANDED.GetCheck());
     c_TVIS_EXPANDEDONCE.EnableWindow(enable && c_m_TVIS_EXPANDEDONCE.GetCheck());
     //c_TVIS_FOCUSED.EnableWindow(enable && c_m_TVIS_FOCUSED.GetCheck());
     c_TVIS_OVERLAYMASK.EnableWindow(enable && c_m_TVIS_OVERLAYMASK.GetCheck());
     c_Overlay.EnableWindow(enable && c_m_TVIS_OVERLAYMASK.GetCheck());
     c_SpinOverlay.EnableWindow(enable && c_m_TVIS_OVERLAYMASK.GetCheck());

     c_TVIS_SELECTED.EnableWindow(enable && c_m_TVIS_SELECTED.GetCheck());
     c_TVIS_STATEIMAGEMASK.EnableWindow(enable && c_m_TVIS_STATEIMAGEMASK.GetCheck());
     c_StateImage.EnableWindow(enable && c_m_TVIS_STATEIMAGEMASK.GetCheck());
     c_SpinStateImage.EnableWindow(enable && c_m_TVIS_STATEIMAGEMASK.GetCheck());

     enable = c_TVIF_CHILDREN.GetCheck() == BST_CHECKED;
     c_cChildren.EnableWindow(enable);
     c_c_cChildren.EnableWindow(enable);

     enable = c_TVIF_HANDLE.GetCheck() == BST_CHECKED;
     c_hItem.EnableWindow(enable);
     c_c_hItem.EnableWindow(enable);

     enable = c_TVIF_IMAGE.GetCheck() == BST_CHECKED;
     c_iImage_Callback.EnableWindow(enable);
     c_c_iImage.EnableWindow(enable);
     c_iImage.EnableWindow(enable &&
                               c_iImage_Callback.GetCheck() == BST_UNCHECKED);
     c_Spin_iImage.EnableWindow(enable &&
                               c_iImage_Callback.GetCheck() == BST_UNCHECKED);

     enable = c_TVIF_PARAM.GetCheck() == BST_CHECKED;
     c_lParam.EnableWindow(enable);
     c_Spin_lParam.EnableWindow(enable);
     c_c_lParam.EnableWindow(enable);

     enable = c_TVIF_SELECTEDIMAGE.GetCheck() == BST_CHECKED;
     c_iSelectedImage_Callback.EnableWindow(enable);
     c_c_iSelectedImage.EnableWindow(enable);
     c_Spin_iSelectedImage.EnableWindow(enable && 
                       c_iSelectedImage_Callback.GetCheck() == BST_UNCHECKED);
     c_iSelectedImage.EnableWindow(enable &&
                       c_iSelectedImage_Callback.GetCheck() == BST_UNCHECKED);

     enable = c_TVIF_TEXT.GetCheck() == BST_CHECKED;
     c_c_pszText.EnableWindow(enable);
     c_pszText_Callback.EnableWindow(enable);
     c_pszText.EnableWindow(enable && 
                              c_pszText_Callback.GetCheck() == BST_UNCHECKED);

     // Now handle the case of setting the checkall state
     
     int state = c_m_TVIS_BOLD.GetCheck();
     if(c_m_TVIS_CUT.GetCheck() != state)
        state = BST_INDETERMINATE;
     if(c_m_TVIS_DROPHILITED.GetCheck() != state)
        state = BST_INDETERMINATE;
     if(c_m_TVIS_EXPANDED.GetCheck() != state)
        state = BST_INDETERMINATE;
     if(c_m_TVIS_EXPANDEDONCE.GetCheck() != state)
        state = BST_INDETERMINATE;
     //if(c_m_TVIS_FOCUSED.GetCheck() != state)
     //   state = BST_INDETERMINATE;
     if(c_m_TVIS_OVERLAYMASK.GetCheck() != state)
        state = BST_INDETERMINATE;
     if(c_m_TVIS_SELECTED.GetCheck() != state)
        state = BST_INDETERMINATE;
     if(c_m_TVIS_STATEIMAGEMASK.GetCheck() != state)
        state = BST_INDETERMINATE;
     c_CheckAll.SetCheck(state);
    }

void CTV_ITEM::OnTextCallback()
{
 enableControls();
}

void CTV_ITEM::OnIimageCallback() 
{
 enableControls();
}

void CTV_ITEM::OnIselectedimageCallback() 
{
 enableControls();
}

void CTV_ITEM::OnMaskTvisBold() 
{
 enableControls();
}

void CTV_ITEM::OnMaskTvisCut() 
{
 enableControls();      
}

void CTV_ITEM::OnMaskTvisDrophilited() 
{
 enableControls();      
}

void CTV_ITEM::OnMaskTvisExpanded() 
{
 enableControls();      
}

void CTV_ITEM::OnMaskTvisExpandedonce() 
{
 enableControls();
        
}

void CTV_ITEM::OnMaskTvisFocused() 
{
 enableControls();
}

void CTV_ITEM::OnMaskTvisOverlaymask() 
{
 enableControls();      
}

void CTV_ITEM::OnMaskTvisSelected() 
{
 enableControls();      
}

void CTV_ITEM::OnMaskTvisStateimagemask() 
{
 enableControls();      
}

void CTV_ITEM::OnTvifChildren() 
{
 enableControls();
        
}

void CTV_ITEM::OnTvifHandle() 
{
 enableControls();      
}

void CTV_ITEM::OnTvifImage() 
{
 enableControls();      
}

void CTV_ITEM::OnTvifParam() 
{
 enableControls();      
}

void CTV_ITEM::OnTvifSelectedimage() 
{
 enableControls();      
}

void CTV_ITEM::OnTvifState() 
{
 enableControls();      
}

void CTV_ITEM::OnTvifText() 
{
 enableControls();      
}

void CTV_ITEM::OnCheckall() 
{
 if(c_CheckAll.GetCheck() == BST_INDETERMINATE)
    { /* cycle back to off */
     c_CheckAll.SetCheck(BST_UNCHECKED);
    } /* cycle back to off */
 c_m_TVIS_BOLD.SetCheck(c_CheckAll.GetCheck());
 c_m_TVIS_CUT.SetCheck(c_CheckAll.GetCheck());
 c_m_TVIS_DROPHILITED.SetCheck(c_CheckAll.GetCheck());
 c_m_TVIS_EXPANDED.SetCheck(c_CheckAll.GetCheck());
 c_m_TVIS_EXPANDEDONCE.SetCheck(c_CheckAll.GetCheck());
 //c_m_TVIS_FOCUSED.SetCheck(c_CheckAll.GetCheck());
 c_m_TVIS_OVERLAYMASK.SetCheck(c_CheckAll.GetCheck());
 c_m_TVIS_SELECTED.SetCheck(c_CheckAll.GetCheck());
 c_m_TVIS_STATEIMAGEMASK.SetCheck(c_CheckAll.GetCheck());
 enableControls();
}

/****************************************************************************
*                          CTV_ITEM::controlsToItem
* Inputs:
*       LPTV_ITEM item: Pointer to item to be filled, or NULL.  If NULL,
*                       the static item for this class is filled
*       BOOL forceTVIF: TRUE to ignore enabled state of TVIF_ controls
* Result: void
*       
* Effect: 
*       Copies the contents of the controls to the item structure.  Fields
*       of the item structure that are not specified by a TVIF_ flag are
*       not modified.
* Notes:
*       The pszText member *must* be a valid pointer to a buffer if the
*       TVIF_TEXT button is checked.  The cchTextMax must also be valid.For a
*       NULL parameter, we supply a pointer to a static buffer and its size
****************************************************************************/

void CTV_ITEM::controlsToItem(LPTV_ITEM pitem, BOOL forceTVIF)
    {
     if(pitem == NULL)
        { /* use default */
         pitem = &item;
         pitem->pszText = text;        // default buffer pointer in case needed
         pitem->cchTextMax = DIM(text); // default length in case used
        } /* use default */

     // Clear the mask so we can set it according to the controls
     pitem->mask = 0;

     if((forceTVIF || c_TVIF_CHILDREN.IsWindowEnabled())
                  && c_TVIF_CHILDREN.GetCheck() == BST_CHECKED)
        { /* children */
         int index = c_cChildren.GetCurSel();
         if(index != CB_ERR)
            { /* has valid value */
             pitem->mask |= TVIF_CHILDREN; // only select if value valid!
             pitem->cChildren = c_cChildren.GetItemData(index);
            } /* has valid value */
        } /* children */

     if((forceTVIF || c_TVIF_HANDLE.IsWindowEnabled())
                && c_TVIF_HANDLE.GetCheck() == BST_CHECKED)
        { /* handle */
         pitem->mask |= TVIF_HANDLE;
         int index = c_hItem.GetCurSel();
         if(index == CB_ERR)
            pitem->hItem = NULL;
         else
            pitem->hItem = (HTREEITEM)c_hItem.GetItemData(index);
        } /* handle */

     if((forceTVIF || c_TVIF_IMAGE.IsWindowEnabled())
                && c_TVIF_IMAGE.GetCheck() == BST_CHECKED)
        { /* image */
         pitem->mask |= TVIF_IMAGE;
         if(c_iImage_Callback.GetCheck() == BST_CHECKED)
            pitem->iImage = I_IMAGECALLBACK;
         else
            pitem->iImage = c_iImage.GetWindowInt();
        } /* image */

     if((forceTVIF || c_TVIF_PARAM.IsWindowEnabled())
                && c_TVIF_PARAM.GetCheck() == BST_CHECKED)
        { /* lParam */
         pitem->mask |= TVIF_PARAM;
         pitem->lParam = c_lParam.GetWindowInt();
        } /* lParam */

     if((forceTVIF || c_TVIF_SELECTEDIMAGE.IsWindowEnabled())
                && c_TVIF_SELECTEDIMAGE.GetCheck() == BST_CHECKED)
        { /* selectedimage */
         pitem->mask |= TVIF_SELECTEDIMAGE;
         if(c_iSelectedImage_Callback.GetCheck() == BST_CHECKED)
            pitem->iSelectedImage = I_IMAGECALLBACK;
         else
            pitem->iSelectedImage = c_iSelectedImage.GetWindowInt();
        } /* selectedimage */

     if((forceTVIF || c_TVIF_STATE.IsWindowEnabled()) 
                && c_TVIF_STATE.GetCheck() == BST_CHECKED)
        { /* state */
         pitem->mask |= TVIF_STATE;
         pitem->stateMask = 0;
         pitem->state = 0;

         if(c_m_TVIS_BOLD.GetCheck() == BST_CHECKED)
            pitem->stateMask |= TVIS_BOLD;
         pitem->state |= (c_TVIS_BOLD.GetCheck() == BST_CHECKED ? TVIS_BOLD : 0);
         if(c_m_TVIS_CUT.GetCheck() == BST_CHECKED)
            pitem->stateMask |= TVIS_CUT;
         pitem->state |= (c_TVIS_CUT.GetCheck() == BST_CHECKED ? TVIS_CUT : 0);
         if(c_m_TVIS_DROPHILITED.GetCheck() == BST_CHECKED)
            pitem->stateMask |= TVIS_DROPHILITED;
         pitem->state |= (c_TVIS_DROPHILITED.GetCheck() == BST_CHECKED ? TVIS_DROPHILITED : 0);

         if(c_m_TVIS_EXPANDED.GetCheck() == BST_CHECKED)
            pitem->stateMask |= TVIS_EXPANDED;
         pitem->state |= (c_TVIS_EXPANDED.GetCheck() == BST_CHECKED ? TVIS_EXPANDED : 0);

         if(c_m_TVIS_EXPANDEDONCE.GetCheck() == BST_CHECKED)
            pitem->stateMask |= TVIS_EXPANDEDONCE;
         pitem->state |= (c_TVIS_EXPANDEDONCE.GetCheck() == BST_CHECKED ? TVIS_EXPANDEDONCE : 0);

         //if(c_m_TVIS_FOCUSED.GetCheck() == BST_CHECKED)
         //   pitem->stateMask |= TVIS_FOCUSED;
         //pitem->state |= (c_TVIS_FOCUSED.GetCheck() == BST_CHECKED ? TVIS_FOCUSED : 0);

         if(c_m_TVIS_OVERLAYMASK.GetCheck() == BST_CHECKED)
            pitem->stateMask |= TVIS_OVERLAYMASK;
         pitem->state |= (c_m_TVIS_OVERLAYMASK.GetCheck() == BST_CHECKED 
                                ? INDEXTOOVERLAYMASK(c_Overlay.GetWindowInt())
                                : 0);

         if(c_m_TVIS_SELECTED.GetCheck() == BST_CHECKED)
            pitem->stateMask |= TVIS_SELECTED;
         pitem->state |= (c_TVIS_SELECTED.GetCheck() == BST_CHECKED ? TVIS_SELECTED : 0);

         if(c_m_TVIS_STATEIMAGEMASK.GetCheck() == BST_CHECKED)
            pitem->stateMask |= TVIS_STATEIMAGEMASK;
         pitem->state |= (c_m_TVIS_STATEIMAGEMASK.GetCheck() == BST_CHECKED 
                        ? INDEXTOSTATEIMAGEMASK(c_StateImage.GetWindowInt())
                        : 0);
        } /* state */

     if((forceTVIF || c_TVIF_TEXT.IsWindowEnabled())
                        && c_TVIF_TEXT.GetCheck() == BST_CHECKED)
        { /* text */
         pitem->mask |= TVIF_TEXT;
         if(c_pszText_Callback.GetCheck() == BST_CHECKED)
            pitem->pszText = LPSTR_TEXTCALLBACK;
         else
            c_pszText.GetWindowText(pitem->pszText, pitem->cchTextMax);
        } /* text */
    }

/****************************************************************************
*                          CTV_ITEM::selectChildren
* Inputs:
*       int code: Child selection code
* Result: int
*       Index of selection that matches or CB_ERR if not found
* Effect: 
*       Selects the item whose ItemData matches code
****************************************************************************/

int CTV_ITEM::selectChildren(int code)
    {
     for(int i = 0; i < c_cChildren.GetCount(); i++)
        { /* search */
         if((int)c_cChildren.GetItemData(i) == code)
            { /* found it */
             c_cChildren.SetCurSel(i);
             return i;
            } /* found it */
        } /* search */
     return CB_ERR;
    }

/****************************************************************************
*                          CTV_ITEM::itemToControls
* Inputs:
*       LPTV_ITEM pitem: Pointer to item, or NULL.  If NULL, values in the
*                       default static item will be used
* Result: void
*       
* Effect: 
*       Copies the state of the item structure to the controls
****************************************************************************/

void CTV_ITEM::itemToControls(LPTV_ITEM pitem)
    {
     if(pitem == NULL)
        pitem = &item;

     if(pitem->pszText == NULL && c_TVIF_TEXT.GetCheck() == BST_CHECKED)
        { /* wants text */
         pitem->pszText = text;
         pitem->cchTextMax = DIM(text);
        } /* wants text */
     
     c_TVIF_CHILDREN.SetCheck(pitem->mask & TVIF_CHILDREN ? BST_CHECKED
                                                          : BST_UNCHECKED);
     c_TVIF_HANDLE.SetCheck(pitem->mask & TVIF_HANDLE     ? BST_CHECKED
                                                          : BST_UNCHECKED);
     c_TVIF_IMAGE.SetCheck(pitem->mask & TVIF_IMAGE       ? BST_CHECKED
                                                          : BST_UNCHECKED);
     c_TVIF_PARAM.SetCheck(pitem->mask & TVIF_PARAM       ? BST_CHECKED
                                                          : BST_UNCHECKED);
     c_TVIF_SELECTEDIMAGE.SetCheck(pitem->mask & TVIF_SELECTEDIMAGE 
                                                          ? BST_CHECKED
                                                          : BST_UNCHECKED);
     c_TVIF_STATE.SetCheck(pitem->mask & TVIF_STATE       ? BST_CHECKED
                                                          : BST_UNCHECKED);
     c_TVIF_TEXT.SetCheck(pitem->mask & TVIF_TEXT         ? BST_CHECKED
                                                          : BST_UNCHECKED);

     c_m_TVIS_BOLD.SetCheck(pitem->stateMask & TVIS_BOLD  ? BST_CHECKED
                                                          : BST_UNCHECKED);
     c_m_TVIS_CUT.SetCheck(pitem->stateMask & TVIS_CUT    ? BST_CHECKED
                                                          : BST_UNCHECKED);
     c_m_TVIS_DROPHILITED.SetCheck(pitem->stateMask & TVIS_DROPHILITED 
                                                          ? BST_CHECKED
                                                          : BST_UNCHECKED);
     c_m_TVIS_EXPANDED.SetCheck(pitem->stateMask & TVIS_EXPANDED 
                                                          ? BST_CHECKED
                                                          : BST_UNCHECKED);
     c_m_TVIS_EXPANDEDONCE.SetCheck(pitem->stateMask & TVIS_EXPANDEDONCE 
                                                          ? BST_CHECKED
                                                          : BST_UNCHECKED);
     //c_m_TVIS_FOCUSED.SetCheck(pitem->stateMask & TVIS_FOCUSED
     //                                                   ? BST_CHECKED
     //                                                   : BST_UNCHECKED);
     c_m_TVIS_OVERLAYMASK.SetCheck(pitem->stateMask & TVIS_OVERLAYMASK
                                                          ? BST_CHECKED
                                                          : BST_UNCHECKED);
     c_m_TVIS_SELECTED.SetCheck(pitem->stateMask & TVIS_SELECTED 
                                                          ? BST_CHECKED
                                                          : BST_UNCHECKED);

     c_TVIS_BOLD.SetCheck(pitem->state & TVIS_BOLD        ? BST_CHECKED
                                                          : BST_UNCHECKED);
     c_TVIS_CUT.SetCheck(pitem->state & TVIS_CUT          ? BST_CHECKED
                                                          : BST_UNCHECKED);
     c_TVIS_DROPHILITED.SetCheck(pitem->state & TVIS_DROPHILITED 
                                                          ? BST_CHECKED
                                                          : BST_UNCHECKED);
     c_TVIS_EXPANDED.SetCheck(pitem->state & TVIS_EXPANDED 
                                                          ? BST_CHECKED
                                                          : BST_UNCHECKED);
     c_TVIS_EXPANDEDONCE.SetCheck(pitem->state & TVIS_EXPANDEDONCE 
                                                          ? BST_CHECKED
                                                          : BST_UNCHECKED);
     //c_TVIS_FOCUSED.SetCheck(pitem->state & TVIS_FOCUSED
     //                                                   ? BST_CHECKED
     //                                                   : BST_UNCHECKED);

     c_Overlay.SetWindowText(OVERLAYMASKTOINDEX(pitem->state));

     c_TVIS_SELECTED.SetCheck(pitem->state & TVIS_SELECTED 
                                                          ? BST_CHECKED
                                                          : BST_UNCHECKED);

     c_StateImage.SetWindowText(STATEIMAGEMASKTOINDEX(pitem->state));


     if(pitem->mask & TVIF_CHILDREN)
        selectChildren(pitem->cChildren);
     else
        c_cChildren.SetCurSel(CB_ERR);

     if(pitem->mask & TVIF_IMAGE)
        { /* image */
         c_iImage_Callback.SetCheck(pitem->iImage == I_IMAGECALLBACK
                                                ? BST_CHECKED
                                                : BST_UNCHECKED);
         if(pitem->iImage == I_IMAGECALLBACK)
            { /* callback */
             c_iImage.SetWindowText(_T(""));
            } /* callback */
         else
            { /* not callback */
             c_iImage.SetWindowText(pitem->iImage);
            } /* not callback */
         c_iImage.SetWindowText(pitem->iImage);
        } /*  image */
     else
        c_iImage.SetWindowText(_T(""));

     if(pitem->mask & TVIF_PARAM)
        c_lParam.SetWindowText(pitem->lParam);
     else
        c_lParam.SetWindowText(_T(""));

     if(pitem->mask & TVIF_SELECTEDIMAGE)
        { /* selected image */
         c_iSelectedImage_Callback.SetCheck(pitem->iSelectedImage == I_IMAGECALLBACK
                                                ? BST_CHECKED
                                                : BST_UNCHECKED);
         if(pitem->iSelectedImage == I_IMAGECALLBACK)
           { /* callback */
            c_iSelectedImage.SetWindowText(_T(""));
           } /* callback */
        else
           { /* not callback */
            c_iSelectedImage.SetWindowText(pitem->iSelectedImage);
           } /* not callback */
        } /* selected image */
     else
        c_iSelectedImage.SetWindowText(_T(""));

     if(pitem->mask & TVIF_TEXT)
        { /* has text */
         BOOL callback = pitem->pszText == LPSTR_TEXTCALLBACK;
         c_pszText_Callback.SetCheck(callback ? BST_CHECKED : BST_UNCHECKED);
         if(callback)
            c_pszText.SetWindowText(_T(""));
         else
            c_pszText.SetWindowText(pitem->pszText);
        } /* has text */
     else
        { /* no text */
         c_pszText.SetWindowText(_T(""));
        } /* no text */
    }
