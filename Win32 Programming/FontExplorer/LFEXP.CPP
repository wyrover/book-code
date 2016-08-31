// lfexp.cpp : implementation file
//

#include "stdafx.h"
#include "FontExplorer.h"
#include "idcombo.h"
#include "degrees.h"
#include "rotate.h"
#include "logfont.h"
#include "message.h"
#include "sample.h"
#include "fontname.h"
#include "fontmet.h"
#include "lfexp.h"
#include "tm.h"
#include "kp.h"
#include "otm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
static int CALLBACK LoadFontProc(const LOGFONT * elf, 
                                 const TEXTMETRIC * ntm, 
                                 DWORD type, 
                                 LPARAM lParam);


static IDData t_Weight[] = {
        { IDS_FW_DONTCARE,     0},
        { IDS_FW_THIN,       100},
        { IDS_FW_EXTRALIGHT, 200},
        { IDS_FW_ULTRALIGHT, 200},
        { IDS_FW_LIGHT,      300},
        { IDS_FW_NORMAL,     400},
        { IDS_FW_REGULAR,    400},
        { IDS_FW_MEDIUM,     500},
        { IDS_FW_SEMIBOLD,   600},
        { IDS_FW_DEMIBOLD,   600},
        { IDS_FW_BOLD,       700},
        { IDS_FW_EXTRABOLD,  800},
        { IDS_FW_ULTRABOLD,  800},
        { IDS_FW_HEAVY,      900},
        { IDS_FW_BLACK,      900},
        { 0, 0} // EOT
                           };

static IDData t_CharSet[] = {
        { IDS_ANSI_CHARSET, ANSI_CHARSET},
        { IDS_DEFAULT_CHARSET, DEFAULT_CHARSET},
        { IDS_SYMBOL_CHARSET, SYMBOL_CHARSET},
        { IDS_SHIFTJIS_CHARSET, SHIFTJIS_CHARSET},
        { IDS_GB2312_CHARSET, GB2312_CHARSET},
        { IDS_HANGEUL_CHARSET, HANGEUL_CHARSET},
        { IDS_CHINESEBIG5_CHARSET, CHINESEBIG5_CHARSET},
        { IDS_OEM_CHARSET, OEM_CHARSET},
        { 0, 0}, // EOT
                            };

static IDData t_CharSet95[] = {
        { IDS_JOHAB_CHARSET, JOHAB_CHARSET},
        { IDS_HEBREW_CHARSET, HEBREW_CHARSET},
        { IDS_ARABIC_CHARSET, ARABIC_CHARSET},
        { IDS_GREEK_CHARSET, GREEK_CHARSET},
        { IDS_TURKISH_CHARSET, TURKISH_CHARSET},
        { IDS_THAI_CHARSET, THAI_CHARSET},
        { IDS_EASTEUROPE_CHARSET, EASTEUROPE_CHARSET},
        { IDS_RUSSIAN_CHARSET, RUSSIAN_CHARSET},
        { IDS_MAC_CHARSET, MAC_CHARSET},
        { IDS_BALTIC_CHARSET, BALTIC_CHARSET},
        { 0, 0 } // EOT
                              };

static IDData t_OutPrecision[] = {
        { IDS_OUT_CHARACTER_PRECIS, OUT_CHARACTER_PRECIS},
        { IDS_OUT_DEFAULT_PRECIS, OUT_DEFAULT_PRECIS},
        { IDS_OUT_DEVICE_PRECIS, OUT_DEVICE_PRECIS},
        { IDS_OUT_OUTLINE_PRECIS, OUT_OUTLINE_PRECIS},
        { IDS_OUT_RASTER_PRECIS, OUT_RASTER_PRECIS},
        { IDS_OUT_STRING_PRECIS, OUT_STRING_PRECIS},
        { IDS_OUT_STROKE_PRECIS, OUT_STROKE_PRECIS},
        { IDS_OUT_TT_ONLY_PRECIS, OUT_TT_ONLY_PRECIS},
        { IDS_OUT_TT_PRECIS, OUT_TT_PRECIS},
        { 0, 0 } // EOT
                                 };

static IDData t_ClipPrecision[] = {
        { IDS_CLIP_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS},
        { IDS_CLIP_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS},
        { IDS_CLIP_STROKE_PRECIS, CLIP_STROKE_PRECIS},
        { IDS_CLIP_MASK, CLIP_MASK},
        { IDS_CLIP_EMBEDDED, CLIP_EMBEDDED},
        { IDS_CLIP_LH_ANGLES, CLIP_LH_ANGLES},
        { IDS_CLIP_TT_ALWAYS, CLIP_TT_ALWAYS},
        { 0, 0} // EOT
                                  };

static IDData t_Pitch[] = {
        { IDS_DEFAULT_PITCH, DEFAULT_PITCH},
        { IDS_FIXED_PITCH, FIXED_PITCH},
        { IDS_VARIABLE_PITCH, VARIABLE_PITCH},
        { 0, 0 } // EOT
                           };

static IDData t_Family[] = {
        { IDS_FF_DECORATIVE, FF_DECORATIVE},
        { IDS_FF_DONTCARE,   FF_DONTCARE},
        { IDS_FF_MODERN,     FF_MODERN},
        { IDS_FF_ROMAN,      FF_ROMAN},
        { IDS_FF_SCRIPT,     FF_SCRIPT},
        { IDS_FF_SWISS,      FF_SWISS}, 
        { 0, 0} // EOT
                           };

static IDData t_Quality[] = {
        { IDS_DEFAULT_QUALITY, DEFAULT_QUALITY},
        { IDS_DRAFT_QUALITY,   DRAFT_QUALITY},
        { IDS_PROOF_QUALITY,   PROOF_QUALITY},
        { 0, 0 } // EOT
                            };

/////////////////////////////////////////////////////////////////////////////
// CLogFontExplorer dialog


CLogFontExplorer::CLogFontExplorer(CWnd* pParent /*=NULL*/)
        : CDialog(CLogFontExplorer::IDD, pParent)
{
 memset(&lf, 0, sizeof(LOGFONT));
 font = NULL;
        //{{AFX_DATA_INIT(CLogFontExplorer)
        //}}AFX_DATA_INIT
}


void CLogFontExplorer::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CLogFontExplorer)
        DDX_Control(pDX, IDC_DATA, c_Data);
        DDX_Control(pDX, IDC_FACENAME, c_lfFaceName);
        DDX_Control(pDX, IDC_SAMPLE, c_Sample);
        DDX_Control(pDX, IDC_RESULT, c_Result);
        DDX_Control(pDX, IDC_LFWIDTH, c_lfWidth);
        DDX_Control(pDX, IDC_LFHEIGHT, c_lfHeight);
        DDX_Control(pDX, IDC_LFESCAPEMENT, c_lfEscapement);
        DDX_Control(pDX, IDC_LFORIENTATION, c_lfOrientation);
        DDX_Control(pDX, IDC_SPINORIENTATION, c_SpinOrientation);
        DDX_Control(pDX, IDC_SPINESCAPEMENT, c_SpinEscapement);
        DDX_Control(pDX, IDC_c_lf_Orientation, c_c_lfOrientation);
        DDX_Control(pDX, IDC_GM_ADVANCED, c_GM_ADVANCED);
        DDX_Control(pDX, IDC_LFFAMILY, c_lfFamily);
        DDX_Control(pDX, IDC_LFPITCH, c_lfPitch);
        DDX_Control(pDX, IDC_LFWEIGHT, c_lfWeight);
        DDX_Control(pDX, IDC_LFQUALITY, c_lfQuality);
        DDX_Control(pDX, IDC_LFOUTPRECISION, c_lfOutPrecision);
        DDX_Control(pDX, IDC_LFCHARSET, c_lfCharSet);
        DDX_Control(pDX, IDC_LFCLIPPRECISION, c_lfClipPrecision);
        DDX_Control(pDX, IDC_LFIUNDERLINE, c_lfUnderline);
        DDX_Control(pDX, IDC_LFITALIC, c_lfItalic);
        DDX_Control(pDX, IDC_LFISTRIKEOUT, c_lfStrikeOut);
        //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLogFontExplorer, CDialog)
        //{{AFX_MSG_MAP(CLogFontExplorer)
        ON_WM_DESTROY()
        ON_BN_CLICKED(IDC_CREATE, OnCreateLogFont)
        ON_CBN_SELENDOK(IDC_LFFAMILY, OnSelendokLffamily)
        ON_WM_VSCROLL()
        ON_BN_CLICKED(IDC_GM_ADVANCED, OnGmAdvanced)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogFontExplorer message handlers

/****************************************************************************
*                           CLogFontExplorer::Is95
* Result: BOOL
*       TRUE if running on Win95
*       FALSE if running on Win/NT
****************************************************************************/

BOOL CLogFontExplorer::Is95()
    {
     OSVERSIONINFO info;
     info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
     ::GetVersionEx(&info);
     return (info.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS);
    }

/****************************************************************************
*                               controls_to_lf
* Result: void
*       
* Effect: 
*       Loads the LOGFONT structure lf from the controls
****************************************************************************/

void CLogFontExplorer::controls_to_lf()
    {
     lf.lfHeight = GetDlgItemInt(IDC_LFHEIGHT);

     lf.lfWidth  = GetDlgItemInt(IDC_LFWIDTH);

     lf.lfEscapement = c_lfEscapement.GetWindowInt();

     if(c_GM_ADVANCED.GetCheck() || Is95())
        { /* esc == orient */
         lf.lfOrientation = lf.lfEscapement;
        } /* esc == orient */
     else
        { /* esc != orient */
         lf.lfOrientation = c_lfOrientation.GetWindowInt();
        } /* esc != orient */

     int index = c_lfWeight.GetCurSel();
     DWORD data = c_lfWeight.GetItemData(index);
     lf.lfWeight = (LONG)data;

     lf.lfItalic = c_lfItalic.GetCheck();
     lf.lfUnderline = c_lfUnderline.GetCheck();
     lf.lfStrikeOut = c_lfStrikeOut.GetCheck();

     index = c_lfCharSet.GetCurSel();
     data = c_lfCharSet.GetItemData(index);
     lf.lfCharSet = (BYTE)data;

     index = c_lfOutPrecision.GetCurSel();
     data = c_lfOutPrecision.GetItemData(index);
     lf.lfOutPrecision = (BYTE)data;

     index = c_lfClipPrecision.GetCurSel();
     data = c_lfClipPrecision.GetItemData(index);
     lf.lfClipPrecision = (BYTE)data;

     index = c_lfQuality.GetCurSel();
     data = c_lfQuality.GetItemData(index);
     lf.lfQuality = (BYTE)data;

     index = c_lfPitch.GetCurSel();
     data = c_lfPitch.GetItemData(index);

     lf.lfPitchAndFamily = (BYTE)data;

     index = c_lfFamily.GetCurSel();
     data = c_lfFamily.GetItemData(index);
     lf.lfPitchAndFamily |= (BYTE)data;

     index = c_lfFaceName.GetCurSel();
     data = c_lfFaceName.GetItemData(index);
     if(data == (DWORD)-1)
        { /* no font */
         lstrcpy(lf.lfFaceName, _T(""));
        } /* no font */
     else
        { /* load it */
         CString s;
         c_lfFaceName.GetWindowText(s);
         lstrcpy(lf.lfFaceName, s);
        } /* load it */
    }

BOOL CLogFontExplorer::OnInitDialog() 
{

        CDialog::OnInitDialog();
        
        CString s;
        s.LoadString(IDS_SAMPLE);
        c_Sample.SetWindowText(s);

        c_lfWeight.AddStrings(t_Weight, IDS_FW_DONTCARE);
        
        c_lfCharSet.AddStrings(t_CharSet, IDS_ANSI_CHARSET);

        if(Is95())
           c_lfCharSet.AddStrings(t_CharSet95);
        
        if(Is95())
           { /* enabled */
            c_lfOrientation.ShowWindow(SW_HIDE);
            c_c_lfOrientation.EnableWindow(FALSE);
            c_SpinOrientation.EnableWindow(FALSE);
           } /* enabled */
        else
           { /* disabled */
            c_lfOrientation.ShowWindow(SW_SHOW);
            c_c_lfOrientation.EnableWindow(TRUE);
            c_SpinOrientation.EnableWindow(TRUE);
           } /* disabled */

        c_GM_ADVANCED.EnableWindow(!Is95());

        c_SpinOrientation.SetBuddy(&c_lfOrientation);
        c_SpinOrientation.SetRange(-3600, 3600);
        c_SpinOrientation.SetPos(0);

        c_SpinEscapement.SetBuddy(&c_lfEscapement);
        c_SpinEscapement.SetRange(-3600, 3600);
        c_SpinEscapement.SetPos(0);

        c_lfOutPrecision.AddStrings(t_OutPrecision, IDS_OUT_DEFAULT_PRECIS);
        c_lfClipPrecision.AddStrings(t_ClipPrecision, IDS_CLIP_DEFAULT_PRECIS);
        c_lfPitch.AddStrings(t_Pitch, IDS_DEFAULT_PITCH);
        c_lfFamily.AddStrings(t_Family, IDS_FF_DONTCARE);
        c_lfQuality.AddStrings(t_Quality, IDS_DEFAULT_QUALITY);

        CClientDC dc(this); // sort-of-randomly-chosen DC
        ::EnumFontFamilies(dc.m_hDC, NULL, ::LoadFontProc, (LPARAM)this);


        s.LoadString(IDS_NONE);
        int index = c_lfFaceName.AddString(s);
        c_lfFaceName.SetItemData(index, (DWORD)-1);

        index = c_lfFaceName.FindString(-1, _T("Arial"));
        if(index == CB_ERR)
           index = 0;
        c_lfFaceName.SetCurSel(index);

        setMessage(0);

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

/****************************************************************************
*                         CLogFontExplorer::OnDestroy
* Result: void
*       
* Effect: 
*       Deletes any resources consumed
****************************************************************************/

void CLogFontExplorer::OnDestroy() 
{
        CDialog::OnDestroy();
        
        if(font != NULL)
           ::DeleteObject(font);
        
}

/****************************************************************************
*                      CLogFontExplorer::OnCreateLogFont
* Result: void
*       
* Effect: 
*       Creates a logfont from the parameters specified, or reports an
*       error.
****************************************************************************/

void CLogFontExplorer::OnCreateLogFont() 
    {
     controls_to_lf();
     HFONT f = ::CreateFontIndirect(&lf);
     if(f == NULL)
        { /* error */
         DWORD err = ::GetLastError();
         setMessage(err);
        } /* error */
     else
        { /* success */
         LOGFONT logf;

         if(font != NULL)
            ::DeleteObject(font);
         font = f;
         c_Sample.SetFont(CFont::FromHandle(font));

         c_Data.DeleteAllItems();

         // Note that we want to get the stats on the actual font created
         // instead of the creation parameters themselves

         CFont * cf = CFont::FromHandle(f);

         cf->GetObject(sizeof(LOGFONT), &logf);

         loadLogFont(&logf, &c_Data, FALSE);


         {
          CClientDC dc(this);
          TEXTMETRIC tm;
          LPOUTLINETEXTMETRIC otm = NULL;

          dc.SelectObject(cf);

          int n = dc.GetOutlineTextMetrics(0, NULL);
          if(n > 0)
             { /* initialize */
              otm = (OUTLINETEXTMETRIC *)new BYTE[n];
              dc.GetOutlineTextMetrics(n, otm);
             } /* initialize */

          dc.GetTextMetrics(&tm);
          loadTextMetrics(&tm, &c_Data);

          loadOutlineTextMetrics(otm, &c_Data);

          loadKerningPairs(dc, &c_Data);
          
          if(otm != NULL)
             delete otm;
         }

         c_Sample.SetFont(cf);
         c_Sample.InvalidateRect(NULL);
        } /* success */
    }

/****************************************************************************
*                     CLogFontExplorer::OnSelendLffamily
* Result: void
*       
* Effect: 
*       NYI
****************************************************************************/

void CLogFontExplorer::OnSelendokLffamily() 
{
        // TODO: Add your control notification handler code here
        
}

/****************************************************************************
*                         CLogFontExplorer::OnVScroll
* Inputs:
*       UINT nSBCode: ignored except for passing to subclass
*       UINT nPos: ignored except for passing to subclass
*       CScrollBar * pScrollBar: ignored except for passing to subclass
* Result: void
*       
* Effect: 
*       Determines which spin control should receive the message
****************************************************************************/

void CLogFontExplorer::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
 if(pScrollBar->m_hWnd == c_SpinOrientation.m_hWnd)
    c_SpinOrientation.OnVScroll(nSBCode, nPos, pScrollBar);
 else
 if(pScrollBar->m_hWnd == c_SpinEscapement.m_hWnd)
    c_SpinEscapement.OnVScroll(nSBCode, nPos, pScrollBar);
 else
    CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

/****************************************************************************
*                       CLogFontExplorer::OnGmAdvanced
* Result: void
*       
* Effect: 
*       If GM_ADVANCED is turned ON (which is possible only on Win/NT)
*       we disabled the lfOrientation control; the lfEscapement is the
*       dominant control
****************************************************************************/

void CLogFontExplorer::OnGmAdvanced() 
{
 if(c_GM_ADVANCED.GetCheck())
    { /* enabled */
     c_lfOrientation.ShowWindow(SW_HIDE);
     c_c_lfOrientation.EnableWindow(FALSE);
     c_SpinOrientation.EnableWindow(FALSE);
     c_Sample.mode = GM_ADVANCED;
    } /* enabled */
 else
    { /* disabled */
     c_lfOrientation.ShowWindow(SW_SHOW);
     c_c_lfOrientation.EnableWindow(TRUE);
     c_SpinOrientation.EnableWindow(TRUE);
     c_Sample.mode = GM_COMPATIBLE;
    } /* disabled */
}

/****************************************************************************
*                                LoadFontProc
* Inputs:
*       ENUMLOGFONT * elf:
*       NEWTEXTMETRIC * ntm:
*       int type:
*       LPARAM lParam: (LPARAM)CLogFontExplorer *
* Result: int
*       
* Effect: 
*       Loads the control
****************************************************************************/

static int CALLBACK LoadFontProc(const LOGFONT * elf, 
                                 const TEXTMETRIC * ntm, 
                                 DWORD type, 
                                 LPARAM lParam)
    {
     CLogFontExplorer * t = (CLogFontExplorer *)lParam;

     int index = t->c_lfFaceName.AddString(elf->lfFaceName);
     t->c_lfFaceName.SetItemData(index, type);

//     if(type & TRUETYPE_FONTTYPE)
//        { /* truetype font */
//       int index = t->c_lfFaceName.AddString(elf->lfFaceName);
//       t->c_lfFaceName.SetItemData(index, 0);
//      } /* truetype font */
     return TRUE; // continue enumeration
    }

/****************************************************************************
*                        CLogFontExplorer::setMessage
* Inputs:
*       DWORD result: Result to display
* Result: void
*       
* Effect: 
*       Puts the string representing the message in the c_Result box
****************************************************************************/

void CLogFontExplorer::setMessage(DWORD result)
    {
     if(result == 0)
        { /* no message */
         c_Result.SetWindowText(NULL);
         return;
        } /* no message */

     LPTSTR msg;
     if(::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | 
                   FORMAT_MESSAGE_ALLOCATE_BUFFER,
                   NULL, 
                   result, 0, 
                   (LPTSTR)&msg,
                   0,
                   NULL) != 0)
        { /* success */
         c_Result.SetWindowText(msg);
         LocalFree(msg);
        } /* success */
     else
        { /* couldn't report error */
         c_Result.SetWindowText(_T("?"));
        } /* couldn't report error */
     
    }
