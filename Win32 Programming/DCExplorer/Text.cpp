// Text.cpp : implementation file
//

#include "stdafx.h"
#include "idcombo.h"
#include "gcombo.h"
#include "color.h"
#include "DCExplorer.h"
#include "dcvals.h"
#include "intedit.h"
#include "Text.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static IDData stockfonts[] = {
	{ IDS_OEM_FIXED_FONT,  OEM_FIXED_FONT},
	{ IDS_ANSI_FIXED_FONT, ANSI_FIXED_FONT},
	{ IDS_ANSI_VAR_FONT, ANSI_VAR_FONT},
	{ IDS_SYSTEM_FONT,     SYSTEM_FONT},
	{ IDS_DEVICE_DEFAULT_FONT, DEVICE_DEFAULT_FONT},
	{ 0, 0} // EOT
			     };
/////////////////////////////////////////////////////////////////////////////
// CText property page

IMPLEMENT_DYNCREATE(CText, CPropertyPage)

CText::CText() : CPropertyPage(CText::IDD)
{
	//{{AFX_DATA_INIT(CText)
	//}}AFX_DATA_INIT
}

CText::~CText()
{
}

void CText::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CText)
	DDX_Control(pDX, IDC_TEXTCHAREXTRA, c_TextCharExtra);
	DDX_Control(pDX, IDC_SPINCHAREXTRA, c_SpinCharExtra);
	DDX_Control(pDX, IDC_TEXTSAMPLE, c_TextSample);
	DDX_Control(pDX, IDC_TEXTCOLOR, c_TextColor);
	DDX_Control(pDX, IDC_TRANSPARENT, c_Transparent);
	DDX_Control(pDX, IDC_OPAQUE, c_Opaque);
	DDX_Control(pDX, IDC_STOCKFONT, c_StockFont);
	DDX_Control(pDX, IDC_STOCKFONTS, c_StockFonts);
	DDX_Control(pDX, IDC_CHANGEFONT, c_ChangeFont);
	DDX_Control(pDX, IDC_FONTNAME, c_FontName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CText, CPropertyPage)
	//{{AFX_MSG_MAP(CText)
	ON_BN_CLICKED(IDC_CHANGEFONT, OnChangefont)
	ON_BN_CLICKED(IDC_CUSTOMFONT, OnCustomfont)
	ON_BN_CLICKED(IDC_STOCKFONT, OnStockfont)
	ON_CBN_SELENDOK(IDC_STOCKFONTS, OnSelendokStockfonts)
	ON_BN_CLICKED(IDC_TEXTCOLOR, OnTextcolor)
	ON_BN_CLICKED(IDC_OPAQUE, OnOpaque)
	ON_BN_CLICKED(IDC_TRANSPARENT, OnTransparent)
	ON_WM_CTLCOLOR()
	ON_CBN_SELENDOK(IDC_TEXTCOLOR, OnSelendokTextcolor)
	ON_EN_CHANGE(IDC_TEXTCHAREXTRA, OnChangeTextcharextra)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CText message handlers


/****************************************************************************
*			    CText::getWeightString
* Inputs:
*       int weight: Weight of font
* Result: CString
*       String which is the weight
* Effect: 
*       Loads the string and uses a temporary CString object to return the
*	value
****************************************************************************/

CString CText::getWeightString(int weight)
    {
     CString s;
     s = _T("");

     switch(weight)
        { /* decode */
	 case FW_THIN:
		 s.LoadString(IDS_THIN);
		 break;
	 case FW_EXTRALIGHT:
		 s.LoadString(IDS_EXTRALIGHT);
		 break;
	 case FW_LIGHT:
		 s.LoadString(IDS_LIGHT);
		 break;
	 case FW_NORMAL:
		 s.LoadString(IDS_NORMAL);
		 break;
	 case FW_MEDIUM:
		 s.LoadString(IDS_MEDIUM);
		 break;
	 case FW_SEMIBOLD:
		 s.LoadString(IDS_SEMIBOLD);
		 break;
	 case FW_BOLD:
		 s.LoadString(IDS_BOLD);
		 break;
	 case FW_EXTRABOLD:
		 s.LoadString(IDS_EXTRABOLD);
		 break;
	 case FW_HEAVY:
		 s.LoadString(IDS_HEAVY);
		 break;
	 default:
		 s.Format("(%d)", weight);
		 break;
	} /* decode */

     return s;
    }

void CText::OnChangefont() 
{
 LOGFONT lf;
 CFontDialog fd(&lf);

 if(font != NULL)
    { /* has font */
     font->GetObject(sizeof(lf), &lf);
     fd.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT;
    } /* has font */
 else
    { /* no font */
     memset(&lf, 0, sizeof(lf));
    } /* no font */

 fd.m_cf.rgbColors = c_TextColor.GetCurItem();

 if(fd.DoModal())
    { /* OK */
     c_TextColor.Select(fd.m_cf.rgbColors);
    } /* OK */

 if(font != NULL)
    delete font;
 
 font = new CFont;

 if(!font->CreateFontIndirect(&fd.m_lf))
    { /* failed */
     // NYI: MessageBox
     CString s;
     // s.LoadString(IDS_DEFAULTFONT);
     setFontName();
    } /* failed */
 else
    { /* success */
     CString s;
     setFontName();
    } /* success */
	
}

void CText::setFontName()
   {
    CString s;
    LOGFONT lf;
    font->GetObject(sizeof(lf), &lf);
    s.Format("%s %d %s", lf.lfFaceName,
     				abs(lf.lfHeight),
				getWeightString(lf.lfWeight));
    c_FontName.SetWindowText(s);
    c_TextSample.SetFont(font);
   }

void CText::OnCustomfont() 
{
 updateControls();
}

void CText::OnStockfont() 
{
 updateControls();
}

void CText::OnSelendokStockfonts() 
{
 updateControls();
}

void CText::OnTextcolor() 
{
	// TODO: Add your control notification handler code here
	
}

void CText::OnOpaque() 
{
 c_TextSample.InvalidateRect(NULL);
	
}

void CText::OnTransparent() 
{
 c_TextSample.InvalidateRect(NULL);
	
}

void CText::OnOK() 
{
	CPropertyPage::OnOK();

 	dcv->lastpage = ((CPropertySheet *)GetParent())->GetActiveIndex();

 	dcv->fontsel = c_StockFont.GetCheck() ? SEL_STOCK : SEL_CUSTOM;
	dcv->stockfont = c_StockFonts.GetCurItem();
	dcv->font = font;

	dcv->BkMode = dcv->current_bkmode;
	dcv->textcolor = c_TextColor.GetCurItem();
	dcv->textcharextra = c_TextCharExtra.GetVal();
}

void CText::updateControls()
    {
     if(c_StockFont.GetCheck())
        { /* stock font */
	 c_StockFonts.EnableWindow(TRUE);
	 c_ChangeFont.EnableWindow(FALSE);
	 int sel = c_StockFonts.GetCurSel();
	 if(sel != CB_ERR)
	    { /* has selection */
	     CString s;
	     CFont stockfont;

	     stockfont.CreateStockObject(c_StockFonts.GetCurItem());
	     c_StockFonts.GetLBText(sel, s);
	     c_FontName.SetWindowText(s);
	     c_TextSample.SetFont(&stockfont);
	    } /* has selection */
	} /* stock font */
     else
        { /* real font */
	 c_StockFonts.EnableWindow(FALSE);
	 c_ChangeFont.EnableWindow(TRUE);

	 if(font != NULL)
	    { /* show font */
	     setFontName();
	    } /* show font */
	 else
	    { /* no font */
	     CFont stockfont;
	     stockfont.CreateStockObject(c_StockFonts.GetCurItem());
	     c_FontName.SetWindowText(_T(""));
	     c_TextSample.SetFont(&stockfont);
	    } /* no font */
	 
	} /* real font */
	 
    }

BOOL CText::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
 	c_StockFonts.AddStrings(stockfonts);
	c_StockFonts.Select(dcv->stockfont);

	CheckRadioButton(IDC_CUSTOMFONT, IDC_STOCKFONT, 
			dcv->fontsel == SEL_STOCK ? IDC_STOCKFONT : IDC_CUSTOMFONT);

	font  = dcv->font;
	pen   = dcv->pen;
	brush = dcv->brush;
	
	int bkmode = (dcv->current_bkmode == 0 ? dcv->BkMode : dcv->current_bkmode);
	CheckRadioButton(IDC_OPAQUE, IDC_TRANSPARENT, 
			(dcv->BkMode == OPAQUE ? IDC_OPAQUE : IDC_TRANSPARENT));

	c_TextColor.Load(dcv->textcolor);

	c_TextCharExtra.SetVal(dcv->textcharextra);

	c_SpinCharExtra.SetRange(-8, 25);

	updateControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CText::OnCancel() 
{
	CPropertyPage::OnCancel();

 if(font != NULL)
    font->DeleteObject();
 if(brush != NULL)
    brush->DeleteObject();
 if(pen != NULL)
    pen->DeleteObject();
}

HBRUSH CText::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(c_TextSample.m_hWnd != NULL &&  // handle autobuddy early msgs
	   c_TextColor.m_hWnd != NULL &&
	   c_Opaque.m_hWnd != NULL &&
	   c_TextCharExtra.m_hWnd != NULL &&
	   pWnd->m_hWnd == c_TextSample.m_hWnd)
	   { /* text sample */
	    pDC->SetTextColor(c_TextColor.GetCurItem());
	    pDC->SetBkMode(c_Opaque.GetCheck() ? OPAQUE : TRANSPARENT);
	    pDC->SetBkColor(RGB(255,255,255));
	    pDC->SetTextCharacterExtra(c_TextCharExtra.GetVal());
	   } /* text sample */
	
	return hbr;
}

void CText::OnSelendokTextcolor() 
{
 c_TextSample.InvalidateRect(NULL);
	
}

/****************************************************************************
*                             CText::OnSetActive
* Result: BOOL
*       As returned by superclass
* Effect: 
*       Makes sure current control settings are consistent with other pages.
****************************************************************************/

BOOL CText::OnSetActive() 
{
	switch(dcv->current_bkmode)
	   { /* change mode */
	    case OPAQUE:
		    CheckRadioButton(IDC_OPAQUE, IDC_TRANSPARENT, IDC_OPAQUE);
		    break;
	    case TRANSPARENT:
		    CheckRadioButton(IDC_OPAQUE, IDC_TRANSPARENT, IDC_TRANSPARENT);
		    break;
	   } /* change mode */
	
	return CPropertyPage::OnSetActive();
}

BOOL CText::OnKillActive() 
{
 dcv->current_bkmode = c_Transparent.GetCheck() ? TRANSPARENT : OPAQUE; 
	
	return CPropertyPage::OnKillActive();
}

void CText::OnChangeTextcharextra() 
{
 if(c_TextSample.m_hWnd != NULL)
    c_TextSample.InvalidateRect(NULL);
}
