#include "stdafx.h"
#include "logfont.h"
#include "resource.h"
#include "panose.h"

/****************************************************************************
*                               familyToString
* Inputs:
*       int paf: Pitch and family
* Result: CString
*       
* Effect: 
*       ²
****************************************************************************/

CString familyToString(int paf)
    {
     CString s;
     s = _T("");

     switch(paf & 0xF0)
        { /* family */
         case FF_DECORATIVE:
                 s.Format(_T("%02x FF_DECORATIVE"), FF_DECORATIVE);
                 break;
         case FF_DONTCARE:
                 s.Format(_T("%02x FF_DONTCARE"), FF_DONTCARE);
                 break;
         case FF_MODERN:
                 s.Format(_T("%02x FF_MODERN"), FF_MODERN);
                 break;
         case FF_ROMAN:
                 s.Format(_T("%02x FF_ROMAN"), FF_ROMAN);
                 break;
         case FF_SCRIPT:
                 s.Format(_T("%02x FF_SCRIPT"), FF_SCRIPT);
                 break;
         case FF_SWISS:
                 s.Format(_T("%02x FF_SWISS"), FF_SWISS);
                 break;
        } /* family */

     return s;
    }

/****************************************************************************
*                                pitchToString
* Inputs:
*       int paf: Pitch and family
* Result: CString
*       
* Effect: 
*       ²
****************************************************************************/

CString pitchToString(int paf, int mask, int id)
    {
     CString s;
     s = _T("");


     if(paf & mask)
        { /* found it */
         CString t;
         t.LoadString(id);
         s.Format("%02x %s", mask, (LPCTSTR)t);
        } /* found it */

     return s;
    }


/****************************************************************************
*                            insertPitchAndFamily
* Inputs:
*       UINT pval: Pitch and family value
*       CTreeCtrl * tc: Tree control
*       HTREEITEM paf: Pitch and family item
* Result: void
*       
* Effect: 
*       Inserts the p-a-f values in the tree at node paf
****************************************************************************/

void insertPitchAndFamily(UINT pval, CTreeCtrl * tc, HTREEITEM paf)
    {
     CString s;
     if(pval != 0)
         { /* has paf */
          int id;
          CString t;
          id = pval;

          t.LoadString(IDS_PITCH);
          HTREEITEM pitch = tc->InsertItem(t, paf);

          s = pitchToString(id, TMPF_FIXED_PITCH, IDS_TMPF_FIXED_PITCH);
          if(s.GetLength() > 0)
             tc->InsertItem(s, pitch);

          s = pitchToString(id, TMPF_VECTOR, IDS_TMPF_VECTOR);
          if(s.GetLength() > 0)
             tc->InsertItem(s, pitch);

          s = pitchToString(id, TMPF_TRUETYPE, IDS_TMPF_TRUETYPE);
          if(s.GetLength() > 0)
             tc->InsertItem(s, pitch);

          s = pitchToString(id, TMPF_DEVICE, IDS_TMPF_DEVICE);
          if(s.GetLength() > 0)
             tc->InsertItem(s, pitch);

          if( (id & (TMPF_VECTOR | TMPF_TRUETYPE)) == 0)
             { /* raster */
              CString t;
              t.LoadString(IDS_TMPF_RASTER);
              s.Format(_T("%02x %s"), 0, t);
              tc->InsertItem(s, pitch);
             } /* raster */

          s = familyToString(pval);
          if(s.GetLength() > 0)
             tc->InsertItem(s, paf);

         } /* has paf */
     
    }

/****************************************************************************
*                               qualityToString
* Inputs:
*       BYTE lfq: Quality code
* Result: CString
*       
****************************************************************************/

CString qualityToString(BYTE lfq)
    {
     CString s;
     s = _T("");

     switch(lfq)
        { /* lfq */
         case DEFAULT_QUALITY:
                 s = _T("DEFAULT_QUALITY");
                 break;
         case DRAFT_QUALITY:
                 s = _T("DRAFT_QUALITY");
                 break;
         case PROOF_QUALITY:
                 s = _T("PROOF_QUALITY");
                 break;
        } /* lfq */
     return s;
    }

/****************************************************************************
*                            clipprecisionToString
* Inputs:
*       BYTE op: clip precision
* Result: CString
*       
****************************************************************************/

CString clipprecisionToString(BYTE op)
    {
     CString s;
     s = _T("");
     switch(op)
        { /* op */
         case CLIP_CHARACTER_PRECIS:
                 s = _T("CLIP_CHARACTER_PRECIS");
                 break;
         case CLIP_DEFAULT_PRECIS:
                 s = _T("CLIP_DEFAULT_PRECIS");
                 break;
         case CLIP_EMBEDDED:
                 s = _T("CLIP_EMBEDDED");
                 break;
         case CLIP_LH_ANGLES:
                 s = _T("CLIP_LH_ANGLES");
                 break;
         case CLIP_MASK:
                 s = _T("CLIP_MASK");
                 break;
         case CLIP_STROKE_PRECIS:
                 s = _T("CLIP_STROKE_PRECIS");
                 break;
         case CLIP_TT_ALWAYS:
                 s = _T("CLIP_TT_ALWAYS");
                 break;
        } /* op */
     return s;
    }

/****************************************************************************
*                            outprecisionToString
* Inputs:
*       BYTE op: Output precision
* Result: CString
*       
****************************************************************************/

CString outprecisionToString(BYTE op)
    {
     CString s;
     s = _T("");
     switch(op)
        { /* op */
         case OUT_CHARACTER_PRECIS:
                 s = _T("OUT_CHARACTER_PRECIS");
                 break;
         case OUT_DEFAULT_PRECIS:
                 s = _T("OUT_DEFAULT_PRECIS");
                 break;
         case OUT_DEVICE_PRECIS:
                 s = _T("OUT_DEVICE_PRECIS");
                 break;
         case OUT_RASTER_PRECIS:
                 s = _T("OUT_RASTER_PRECIS");
                 break;
         case OUT_STROKE_PRECIS:
                 s = _T("OUT_STROKE_PRECIS");
                 break;
         case OUT_TT_PRECIS:
                 s = _T("OUT_TT_PRECIS");
                 break;
         case OUT_TT_ONLY_PRECIS:
                 s = _T("OUT_TT_ONLY_PRECIS");
                 break;
        } /* op */
     return s;
    }

/****************************************************************************
*                               charsetToString
* Inputs:
*       BYTE cs: charset code
* Result: CString
*       
****************************************************************************/

CString charsetToString(BYTE cs)
    {
     CString s;
     s = _T("");

     switch(cs)
        { /* cs */
         case ANSI_CHARSET:
                s = _T("ANSI_CHARSET");
                break;
         case DEFAULT_CHARSET:
                s = _T("DEFAULT_CHARSET");
                break;
         case SYMBOL_CHARSET:
                s = _T("SYMBOL_CHARSET");
                break;
         case SHIFTJIS_CHARSET:
                s = _T("SHIFTJIS_CHARSET");
                break;
         case OEM_CHARSET:
                s = _T("OEM_CHARSET");
                break;
        } /* cs */
     return s;
    }

/****************************************************************************
*                               weightToString
* Inputs:
*       int weight: Weight of font
* Result: CString
*       String representing the weight
* Effect: 
*       ²
****************************************************************************/

CString weightToString(int weight)
    {
     switch(weight)
        { /* tmWeight */
         case FW_DONTCARE:
                 return CString(_T("FW_DONTCARE"));
         case FW_THIN:
                 return CString(_T("FW_THIN"));
         case FW_EXTRALIGHT:
                 return CString(_T("FW_EXTRALIGHT/FW_ULTRALIGHT"));
         case FW_LIGHT:
                 return CString(_T("FW_LIGHT"));
         case FW_NORMAL:
                 return CString(_T("FW_NORMAL/FW_REGULAR"));
         case FW_MEDIUM:
                 return CString(_T("FW_MEDIUM"));
         case FW_SEMIBOLD:
                 return CString(_T("FW_SEMIBOLD/FW_DEMIBOLD"));
         case FW_BOLD:
                 return CString(_T("FW_BOLD"));
         case FW_EXTRABOLD:
                 return CString(_T("FW_EXTRABOLD/ULTRABOLD"));
         case FW_BLACK:
                 return CString(_T("FW_BLACK/FW_HEAVY"));
        } /* tmWeight */

     int low = (weight / 100) * 100;
     int high = low / 100;

     if(FW_DONTCARE <= low && low <= FW_BLACK &&
        FW_DONTCARE <= high && high <= FW_BLACK)
           { /* make range */
            CString s;
            s.Format(_T("%s..%s"), weightToString(low), weightToString(high));
            return s;
           } /* make range */

     return _T("");
    }

/****************************************************************************
*                                 loadLogFont
* Inputs:
*       CFontDescriptor * desc: Font descriptor
*       CTreeCtrl * tc: Tree control
*       BOOL exlf: TRUE if structure passed in is an ExtLogFont, FALSE 
*                               otherwise
* Result: HTREEITEM
*       Handle to LOGFONT item
* Effect: 
*       Loads the LOGFONT information
****************************************************************************/

HTREEITEM loadLogFont(LOGFONT * lf, CTreeCtrl * tc, BOOL exlf)
    {
     CString s;
     
     HTREEITEM lfi;

#if 0
     if(exlf)
        lfi = tc->InsertItem(_T("EXTLOGFONT"));
     else
        lfi = tc->InsertItem(_T("LOGFONT"));
#else
     lfi = tc->InsertItem(_T("LOGFONT"));
#endif

     s.Format(_T("lfHeight %d"), lf->lfHeight);
     tc->InsertItem(s, lfi);

     s.Format(_T("lfWidth %d"), lf->lfWidth);
     tc->InsertItem(s, lfi);

     s.Format(_T("lfEscapement %d"), lf->lfEscapement);
     tc->InsertItem(s, lfi);
     
     s.Format(_T("lfOrientation %d"), lf->lfOrientation);
     tc->InsertItem(s, lfi);

     s.Format(_T("lfWeight %d"), lf->lfWeight);
     HTREEITEM wt = tc->InsertItem(s, lfi);
     s = weightToString(lf->lfWeight);
     if(s.GetLength() != 0)
        tc->InsertItem(s, wt);

     s.Format(_T("lfItalic %d"), lf->lfItalic);
     tc->InsertItem(s, lfi);

     s.Format(_T("lfUnderline %d"), lf->lfUnderline);
     tc->InsertItem(s, lfi);

     s.Format(_T("lfStrikeOut %d"), lf->lfStrikeOut);
     tc->InsertItem(s, lfi);

     s.Format(_T("lfCharSet %d"), lf->lfCharSet);
     HTREEITEM cs = tc->InsertItem(s, lfi);
        s = charsetToString(lf->lfCharSet);
        tc->InsertItem(s, cs);
        

     s.Format(_T("lfOutPrecision %d"), lf->lfOutPrecision);
     HTREEITEM op = tc->InsertItem(s, lfi);
        s = outprecisionToString(lf->lfOutPrecision);
        tc->InsertItem(s, op);

     s.Format(_T("lfClipPrecision %d"), lf->lfClipPrecision);
     HTREEITEM cp = tc->InsertItem(s, lfi);
         s = clipprecisionToString(lf->lfClipPrecision);
         tc->InsertItem(s, cp);

     s.Format(_T("lfQuality %d"), lf->lfQuality);
     HTREEITEM q = tc->InsertItem(s, lfi);
         s = qualityToString(lf->lfQuality);
         tc->InsertItem(s, q);

     s.Format(_T("lfPitchAndFamily 0x%02x"), lf->lfPitchAndFamily);
     HTREEITEM paf = tc->InsertItem(s, lfi);
     insertPitchAndFamily(lf->lfPitchAndFamily, tc, paf);

     s.Format(_T("lfFaceName \"%s\""), lf->lfFaceName);
     tc->InsertItem(s, lfi);

#if 0
     if(tt)
        { /* TrueType */
         LPEXTLOGFONT elf = (LPEXTLOGFONT)lf;
         
         s.Format(_T("elfFullName \"%s\""), elf->elfFullName);
         tc->InsertItem(s, lfi);

         s.Format(_T("elfStyle \"%s\""), elf->elfStyle);
         tc->InsertItem(s, lfi);

         s.Format(_T("elfVersion %d"), elf->elfVersion);
         tc->InsertItem(s, lfi);

         s.Format(_T("elfStyleSize %d"), elf->elfStyleSize);
         tc->InsertItem(s, lfi);

         s.Format(_T("elfMatch %d"), elf->elfMatch);
         tc->InsertItem(s, lfi);

         s.Format(_T("elfStyleSize %d"), elf->elfStyleSize);
         tc->InsertItem(s, lfi);

         s.Format(_T("elfVendorId %d = [0x%02x 0x%02x 0x%02x 0x%02x]"), 
                                elf->elfVendorId,
                                LOBYTE(LOWORD(elf->elfVendorId)),
                                HIBYTE(LOWORD(elf->elfVendorId)),
                                LOBYTE(HIWORD(elf->elfVendorId)),
                                HIBYTE(HIWORD(elf->elfVendorId)));
         tc->InsertItem(s, lfi);


         s.Format(_T("elfCulture %d"), elf->elfCulture);
         tc->InsertItem(s, lfi);
         
         HTREEITEM pi = tc->InsertItem(_T("elfPanose"), lfi);
         loadPanose(tc, pi, &elf->elfPanose);
        } /* TrueType */
#endif

     return lfi;
    }
