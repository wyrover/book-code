// Raster.cpp : implementation file
//

#include "stdafx.h"
#include "FontExplorer.h"
#include "intedit.h"
#include "Raster.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RASTER_MAX_MULTIPLIER 20
#define VECTOR_MAX_MULTIPLIER 256
#define TRUETYPE_MAX_MULTIPLIER 256

/////////////////////////////////////////////////////////////////////////////
// CRaster dialog


CRaster::CRaster(CWnd* pParent /*=NULL*/)
        : CDialog(CRaster::IDD, pParent)
{
        initialized = FALSE;
        scalable = FALSE;
        //{{AFX_DATA_INIT(CRaster)
        //}}AFX_DATA_INIT
}


void CRaster::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CRaster)
        DDX_Control(pDX, IDC_MULTIPLIER_CAPTION, c_c_Multiplier);
        DDX_Control(pDX, IDC_HEIGHT_CAPTION, c_c_Height);
        DDX_Control(pDX, IDC_VECTOR, c_Vector);
        DDX_Control(pDX, IDC_TRUETYPE, c_TrueType);
        DDX_Control(pDX, IDC_RASTER, c_Raster);
        DDX_Control(pDX, IDC_MULTIPLIER, c_Multiplier);
        DDX_Control(pDX, IDC_FONT_SIZE, c_FontSize);
        DDX_Control(pDX, IDC_FONTS, c_Fonts);
        DDX_Control(pDX, IDC_TEXT, c_Text);
        DDX_Control(pDX, IDC_SPINSIZE, c_SpinSize);
        DDX_Control(pDX, IDC_SAMPLE, c_Sample);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRaster, CDialog)
        //{{AFX_MSG_MAP(CRaster)
        ON_EN_CHANGE(IDC_TEXT, OnChangeText)
        ON_CBN_SELENDOK(IDC_FONTS, OnSelendokFonts)
        ON_EN_CHANGE(IDC_MULTIPLIER, OnChangeMultiplier)
        ON_CBN_SELENDOK(IDC_FONT_SIZE, OnSelendokFontSize)
        ON_BN_CLICKED(IDC_COPY, OnCopy)
        ON_WM_CTLCOLOR()
        ON_BN_CLICKED(IDC_RASTER, OnRaster)
        ON_BN_CLICKED(IDC_TRUETYPE, OnTruetype)
        ON_BN_CLICKED(IDC_VECTOR, OnVector)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRaster message handlers


void CRaster::OnChangeText() 
{
 CString s;
 c_Text.GetWindowText(s);
 c_Sample.SetWindowText(s);
        
}


/****************************************************************************
*                                loadFontProc
* Inputs:
*       ENUMLOGFONT * elf:
*       NEWTEXTMETRIC * ntm:
*       int type:
*       LPARAM lParam: (LPARAM)CRaster *
* Result: int
*       
* Effect: 
*       Loads the control
****************************************************************************/

static int CALLBACK loadFontProc(const LOGFONT * lf, 
                                 const TEXTMETRIC * ntm, 
                                 DWORD type, 
                                 LPARAM lParam)
    {
     CRaster * t = (CRaster *)lParam;

     if((t->fontmask == (DWORD)-1 && type == 0) ||
        (t->fontmask != (DWORD)-1 && (type & t->fontmask)))
        { /* add to list */
         int index = t->c_Fonts.AddString(lf->lfFaceName);
         t->c_Fonts.SetItemData(index, type);
        } /* add to list */

     return TRUE; // continue enumeration
    }

BOOL CRaster::OnInitDialog() 
{
        CDialog::OnInitDialog();
        
        CheckRadioButton(IDC_RASTER, IDC_TRUETYPE, IDC_RASTER);

        c_Text.SetWindowText(_T("W"));
        c_Text.SetLimitText(1);
        
        initialized = TRUE;

        OnRaster();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

/****************************************************************************
*                               ::loadFontSizes
* Inputs:
*       const LOGFONT * lf: LOGFONT for current font
*       const TEXTMETRIC * tm: TEXTMETRIC
*       DWORD type:
*       LPARAM lParam: (LPARAM)(CRaster *)
* Result: int
*       TRUE to continue enumeration
*       FALSE to stop enumeration
* Effect: 
*       Loads up the size combo given the font has been selected
****************************************************************************/

static int CALLBACK loadFontSizes(const LOGFONT * lf, const TEXTMETRIC * tm, DWORD type, LPARAM lParam )
    {
     CRaster * wnd = (CRaster *)lParam;

     CString s;
     s.Format(_T("%3d"), lf->lfHeight);
     int index = wnd->c_FontSize.AddString(s);
     wnd->c_FontSize.SetItemData(index, lf->lfHeight);

     return TRUE;
    }

void CRaster::OnSelendokFonts() 
{
 loadFontSizes();

 setNewFont();
}

/****************************************************************************
*                             CRaster::loadFontSizes
* Result: void
*       
* Effect: 
*       Loads the font size dropdown
****************************************************************************/

void CRaster::loadFontSizes()
    {
     // load the font size control

     CString s;
     c_Fonts.GetWindowText(s);

     c_FontSize.ResetContent();

     CClientDC dc(this);
     EnumFonts(dc.m_hDC, s, ::loadFontSizes, (LPARAM)this);

     c_FontSize.SetCurSel(0);
     
    }

/****************************************************************************
*                             CRaster::setNewFont
* Result: void
*       
* Effect: 
*       Creates a new font and sets it in the display window
****************************************************************************/

void CRaster::setNewFont()
    {
     if(!initialized)
        return;

     CString newname;

     LOGFONT lf;

     c_Fonts.GetWindowText(newname);  // get new font name

     CFont * font = c_Sample.GetFont();

     font->GetObject(sizeof(lf), &lf);

     int x = c_Multiplier.GetVal();

     int sel = scalable  ? 1
                         : c_FontSize.GetItemData(c_FontSize.GetCurSel());

     int size = sel * x;

     if(lstrcmp(lf.lfFaceName, newname) != 0 || size != lf.lfHeight)
        { /* need new font */
         lf.lfHeight = size;
         lstrcpy(lf.lfFaceName, newname);

         CFont newfont;
         if(newfont.CreateFontIndirect(&lf))
            { /* success */
             c_Sample.SetFont(&newfont);
             font->DeleteObject();
             newfont.Detach();
            } /* success */
        } /* need new font */
     
    }

void CRaster::OnChangeMultiplier() 
{
 setNewFont();
}

void CRaster::OnSelendokFontSize() 
{
 setNewFont();
}

void CRaster::OnCopy() 
{
 CClientDC wdc(&c_Sample);  // get static control that surrounds example

 CDC memDC;

 memDC.CreateCompatibleDC(&wdc);

 CBitmap bm;
 CRect r;
 
 c_Sample.GetClientRect(&r);

 CString s;
 c_Sample.GetWindowText(s);

 CFont * font = c_Sample.GetFont();

 wdc.SelectObject(font);

 CSize sz = wdc.GetTextExtent(s);

 bm.CreateCompatibleBitmap(&wdc, sz.cx, sz.cy);
 
 CBitmap * oldbm = memDC.SelectObject(&bm);
 
 memDC.BitBlt(0, 0, sz.cx, sz.cy, &wdc, 0, 0, SRCCOPY);

 OpenClipboard();
 ::EmptyClipboard();
 ::SetClipboardData(CF_BITMAP, bm.m_hObject);
 CloseClipboard();

 memDC.SelectObject(oldbm);
 bm.Detach();  // make sure bitmap not deleted with CBitmap object
}

HBRUSH CRaster::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
        HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
        
        if(pWnd->m_hWnd == c_Sample.m_hWnd)
           { /* change background */
            hbr = (HBRUSH)::GetStockObject(WHITE_BRUSH);
            pDC->SetBkMode(TRANSPARENT);
           } /* change background */
        
        return hbr;
}

/****************************************************************************
*                            CRaster::loadFontSet
* Result: void
*       
* Effect: 
*       Loads the font set based on the current radio button selection
****************************************************************************/

void CRaster::loadFontSet()
    {
     if(c_Raster.GetCheck())
        fontmask = RASTER_FONTTYPE;
     else
     if(c_Vector.GetCheck())
        fontmask = (DWORD)-1;
     else
        fontmask = TRUETYPE_FONTTYPE;


     c_Fonts.ResetContent();

     CClientDC dc(&c_Sample);
     ::EnumFontFamilies(dc.m_hDC, NULL, ::loadFontProc, (LPARAM)this);
     
     c_Fonts.SetCurSel(0);

     c_SpinSize.SetPos(1);
        
     c_FontSize.SetCurSel(0);

     loadFontSizes();

     setNewFont();
    }

void CRaster::OnRaster() 
{
 CString s;
 s.LoadString(IDS_RASTERFONTS);
 SetWindowText(s);

 setScalable(FALSE);

 c_SpinSize.SetRange(1, RASTER_MAX_MULTIPLIER);

 loadFontSet();
}

void CRaster::OnTruetype() 
{
 CString s;
 s.LoadString(IDS_TRUETYPEFONTS);
 SetWindowText(s);

 setScalable(TRUE);

 c_SpinSize.SetRange(1, TRUETYPE_MAX_MULTIPLIER);

 loadFontSet();

 c_SpinSize.SetPos(14);
}

void CRaster::OnVector() 
{
 CString s;
 s.LoadString(IDS_VECTORFONTS);
 SetWindowText(s);

 setScalable(TRUE);

 c_SpinSize.SetRange(1, VECTOR_MAX_MULTIPLIER);

 loadFontSet();
}

/****************************************************************************
*                            CRaster::setScalable
* Inputs:
*       BOOL scalable: TRUE for scalable fonts (Vector, TrueType)
*                      FALSE for nonscalable fonts (Raster)
* Result: void
*       
* Effect: 
*       Sets the display for scalability
****************************************************************************/

void CRaster::setScalable(BOOL scale)
    {
     CString s;

     scalable = scale;

     c_c_Height.ShowWindow(scalable ? SW_HIDE : SW_SHOW);
     c_FontSize.ShowWindow(scalable ? SW_HIDE : SW_SHOW);

     s.LoadString(scalable ? IDS_HEIGHT_CAPTION : IDS_MULTIPLIER);
     c_c_Multiplier.SetWindowText(s);
    }
