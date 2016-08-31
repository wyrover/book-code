/*****************************************************************************
*           Change Log
*  Date     | Change
*-----------+-----------------------------------------------------------------
*  5-Apr-95 | <jmn> Created
*****************************************************************************/
#define STRICT

#include "stdafx.h"
#include "FontObj.h"
#include "makefont.h"
#include "fontstr.h"

#define FONTSECTION "Fonts"

/*****************************************************************************
Note: We make this variable a CPtrArray * and initialize it the first time
in the constructor.  This may seem odd, but in an earlier version we just
declared this an ordinary CPtrArray, which led to serious problems.  It turns
out that the constructors for static instances of this type were being
called in modules that initialized before this module initialized, consequently
the CPtrArray was not initialized and we got all sorts of assertion failures.
By declaring this a compile-time static initialization (= NULL) we can
test in the constructor to see if it has been allocated, and if not, allocate
it the first time the constructor is called.  Very convoluted logic, but
necessitated by deficiencies in how C++ performs initializations!
*****************************************************************************/
CPtrArray * CFontObj::fonttable = NULL;

/****************************************************************************
*                            CFontObj::CFontObj
* Inputs:
*       LPCTSTR prf: profile name in [Fonts] section.  Can be 
*               MAKEINTRESOURCE of resource ID
*       LPCTSTR desc: Human readable description.  Can be 
*               MAKEINTRESOURCE of resource ID
*       LPCTSTR samp: Sample text for font display, or NULL.  Can be
*               MAKEINTRESOURCE of resource ID
* Effect: 
*       Creates a new CFontObj object and initializes it
*       Adds the CFontObj object to the global table of known fonts
****************************************************************************/

CFontObj::CFontObj(LPCTSTR prf, LPCTSTR desc, LPCTSTR samp)
    {
     Profile = prf;
     Description = desc;
     Sample = samp;

     Font = NULL;
     NewFont = NULL;
     ApplyFont = NULL;
     changed = FALSE;
     if(fonttable == NULL)
        fonttable = new CPtrArray();
     fonttable->SetAtGrow(fonttable->GetSize(), (LPVOID)this);
    }

/****************************************************************************
*                            CFontObj::~CFontObj
* Effect: 
*       Frees up all resources held by the font descriptor
****************************************************************************/

CFontObj::~CFontObj()
    {
     DeleteBaseFont();
     DeleteNewFont();
     DeleteApplyFont();
    }

/****************************************************************************
*                            CFontObj::IsSameFont
* Inputs:
*       LPLOGFONT lf: LOGFONT structure
*       CFont * font: CFont object 
* Result: BOOL
*       TRUE if lf == font, FALSE if lf != font
* Notes: 
*       This is used to check font equivalence, such as checking that
*       the LOGFONT obtained from GetCurrentFont is the same as the
*       ApplyFont
****************************************************************************/

BOOL CFontObj::IsSameFont(LPLOGFONT lf, CFont * font)
    {
     LOGFONT fnt;

     if(font == NULL)
        return FALSE;   // no CFont at all
     if(font->GetSafeHandle() == NULL)
        return FALSE;   // no GDI object present
     font->GetObject(sizeof(fnt), (LPVOID)&fnt);
     
     // We compare selected fields of fnt and lf
     if(lstrcmp(lf->lfFaceName, fnt.lfFaceName) != 0)
        return FALSE;  // face names different
     if(lf->lfHeight != fnt.lfHeight)
        return FALSE;
     if(lf->lfItalic != fnt.lfItalic)
        return FALSE;
     if(lf->lfWeight != fnt.lfWeight)
        return FALSE;

     // Some parameters that we can't change we don't check, for example
     // underline, strikeout, orientation, escapement, precision, quality,
     // pitch-and-family and character set

     // it didn't differ in any of the above parameters, call it the same
     return TRUE;
    }

/****************************************************************************
*                          CFontObj::GetCurrentFont
* Result: CFont *
*       If there is an active font dialog, use the font from the dialog
*       If there is a NewFont, use it
*       If there is a base Font, use it
*       Otherwise, NULL
****************************************************************************/

CFont * CFontObj::GetCurrentFont()
    {
     if(fd != NULL)
        { /* use font dialog */
         LOGFONT lf;
         fd->GetCurrentFont(&lf); // get font from CFontDialog
         // Now see if the font from the dialog is the same as our
         // ApplyFont.  If it is, don't do anything; if it is different,
         // create a new font and replace the ApplyFont with it
         if(!IsSameFont(&lf, ApplyFont))
            { /* font changed */
             CFont * f = new CFont();
             if(!f->CreateFontIndirect(&lf))
                { /* creation failed */
                 delete f;
                 // fall thru to lower cases
                } /* creation failed */
             else
                { /* creation succeeded */
                 // Make the new font be the selected font
                 SetApplyFont(f);
                 return f;
                } /* creation succeeded */
            } /* font changed */
         else
            { /* same font */
             return ApplyFont;
            } /* same font */
        } /* use font dialog */

     if(NewFont != NULL)
        return NewFont;

     return Font;  // which may be NULL
    }

/****************************************************************************
*                           CFontObj::GetBaseFont
* Result: CFont *
*       The base font for the font descriptor, or NULL
****************************************************************************/

CFont * CFontObj::GetBaseFont()
    {
     return Font;
    }

/****************************************************************************
*                            CFontObj::GetNewFont
* Result: CFont *
*       The 'new' font, or NULL if there is not a valid new font
****************************************************************************/

CFont * CFontObj::GetNewFont()
    {
     if(NewFont != NULL && NewFont->GetSafeHandle() != NULL)
        return NewFont;
     return NULL;
    }

/****************************************************************************
*                            CFontObj::GetApplyFont
* Result: CFont *
*       The 'Apply' font, or NULL if there is not a valid Apply font
****************************************************************************/

CFont * CFontObj::GetApplyFont()
    {
     if(ApplyFont != NULL && ApplyFont->GetSafeHandle() != NULL)
        return ApplyFont;
     return NULL;
    }

/****************************************************************************
*                          CFontObj::DeleteBaseFont
* Result: void
*       
* Effect: 
*       If there is a font and a CFont in the base 'Font' member, delete
*       the GDI object and free the CFont
****************************************************************************/

void CFontObj::DeleteBaseFont()
    {
     if(Font != NULL && Font->GetSafeHandle() != NULL)
        Font->DeleteObject();  // documentation isn't clear on this
     if(Font != NULL)
        delete Font;     
     Font = NULL;
    }

/****************************************************************************
*                         CFontObj::DeleteApplyFont
* Result: void
*       
* Effect: 
*       If there is a font and a CFont in the 'ApplyFont' member, delete
*       the GDI object and free the CFont
****************************************************************************/

void CFontObj::DeleteApplyFont()
    {
     if(ApplyFont != NULL && ApplyFont->GetSafeHandle() != NULL)
        ApplyFont->DeleteObject();  // documentation isn't clear on this
     if(ApplyFont != NULL)
        delete ApplyFont;     
     ApplyFont = NULL;
    }

/****************************************************************************
*                           CFontObj::SetBaseFont
* Inputs:
*       CFont * font: New font to be set, or NULL to delete base font
* Result: void
*       
* Effect: 
*       Sets the indicated font as the contents of the base font.  If there
*       was already a font object in the base font, it is deleted
****************************************************************************/

void CFontObj::SetBaseFont(CFont * newbasefont)
    {
     DeleteBaseFont();
     Font = newbasefont;
    }

/****************************************************************************
*                           CFontObj::SetApplyFont
* Inputs:
*       CFont * font: New font to be set, or NULL to delete Apply font
* Result: void
*       
* Effect: 
*       Sets the indicated font as the contents of the Apply font.  If there
*       was already a font object in the Apply font, it is deleted
****************************************************************************/

void CFontObj::SetApplyFont(CFont * newApplyfont)
    {
     DeleteApplyFont();
     ApplyFont = newApplyfont;
    }

/****************************************************************************
*                           CFontObj::SetNewFont
* Inputs:
*       CFont * font: New font to be set, or NULL to delete NewFont
* Result: void
*       
* Effect: 
*       Sets the indicated font as the contents of the base font.  If there
*       was already a font object in the base font, it is deleted
****************************************************************************/

void CFontObj::SetNewFont(CFont * newNewFont)
    {
     DeleteNewFont();
     NewFont = newNewFont;
    }

/****************************************************************************
*                    CFontObj::ReplaceBaseFontWithNewFont
* Result: void
*       
* Effect: 
*       Replaces the base font with the new font, resetting the NewFont
*       to NULL.  The existing base font, if any, is deleted and its GDI
*       resources are freed.  If there is no NewFont (NULL), this has
*       no effect.
****************************************************************************/

void CFontObj::ReplaceBaseFontWithNewFont()
    {
     if(NewFont == NULL)
        return;
     DeleteBaseFont();
     Font = NewFont;
     NewFont = NULL;
     DeleteApplyFont();
    }

/****************************************************************************
*                          CFontObj::SetFontDialog
* Inputs:
*       CFontDialog * fd: The font dialog to set, or NULL to clear it
* Result: void
*       
* Effect: 
*       Sets (or clears, if fd==NULL) the font dialog for the current
*       font entry.  
* Notes:
*       No more than one font entry in the font table can have a fontdialog
*       entry.  We do not store this as a static member variable because
*       it is used when we want the current font to cause the selected font
*       from the font dialog to be used.  This is used for the implementation
*       of the "Apply" button.
****************************************************************************/

void CFontObj::SetFontDialog(CFontDialog * dlg)
    {
     fd = dlg;
     if(fd == NULL)
        DeleteApplyFont();
    }

/****************************************************************************
*                          CFontObj::DeleteNewFont
* Result: void
*       
* Effect: 
*       Deletes the object in the NewFont slot and frees up its resources
****************************************************************************/

void CFontObj::DeleteNewFont()
    {
     if(NewFont != NULL && NewFont->GetSafeHandle() != NULL)
        NewFont->DeleteObject();  // documentation isn't clear on this
     if(NewFont != NULL)
        delete NewFont;
     NewFont = NULL;
    }

/****************************************************************************
*                           CFontObj::GetFontObj
* Inputs:
*       int i: Index of font
* Result: CFontObj *
*       Reference to the font descriptor, or NULL if i is out of range
****************************************************************************/

CFontObj * CFontObj::GetFontObj(int i)
    {
     if(i >= GetCount())
        return NULL;
     return (CFontObj *)(*fonttable)[i];
    }

/****************************************************************************
*                           CFontObj::FreeAllFonts
* Result: void
*       
* Effect: 
*       Deletes all fonts in the font table and then deletes the font table
****************************************************************************/

void CFontObj::FreeAllFonts()
    {
     fonttable->RemoveAll();
    }

/****************************************************************************
*                             CFontObj::SaveFont
* Inputs:
*       LOGFONT * lf: Logical font to save, or NULL to delete [section]name=
*       HDC dc: Display context used for scaling info
* Result: void
*       
* Effect: 
*       Writes the font data out for later recall
****************************************************************************/

void CFontObj::SaveFont(LOGFONT * lf, CDC * dc)
    {
     char line[256];
     

     if(ProfileString.GetLength() == 0)
        ProfileString = getString(Profile);
     
     if(lf == NULL)
        { /* delete font descr */
         ::WritePrivateProfileString(FONTSECTION, ProfileString, NULL, 
                                                AfxGetApp()->m_pszProfileName);
         return;
        } /* delete font descr */

     FontToString(line, lf, dc, TRUE);

     ::WritePrivateProfileString(FONTSECTION, ProfileString, line, 
                                        AfxGetApp()->m_pszProfileName);
    }

/****************************************************************************
*                             CFontObj::SaveFonts
* Result: void
*       
* Effect: 
*       Saves all the fonts in the [Fonts] section of the application's
*       .INI file (or registry, in the future)
****************************************************************************/

void CFontObj::SaveFonts()
    {
     CDC * dc = AfxGetApp()->m_pMainWnd->GetDC();
     LOGFONT lf;

     for(int i=0; i < GetCount(); i++)
        { /* save each */
         ((CFontObj *)(*fonttable)[i])->SaveFont( &lf, dc);
        } /* save each */

     AfxGetApp()->m_pMainWnd->ReleaseDC(dc);
    }

/****************************************************************************
*                             CFontObj::MakeFont
* Inputs:
*       LPCTSTR facename: Name of font
*       int height: Height in points
*       int weight: Weight, e.g., FW_NORMAL, FW_BOLD
*       CDC * dc: DC used to convert to output device height
* Result: BOOL
*       TRUE if successful
*       FALSE if error
* Effect: 
*       Creates the font and assigns it to the Font member
****************************************************************************/

BOOL CFontObj::MakeFont(LPCTSTR facename, int height, int weight, CDC * dc)
    {
     CFont * font = new CFont();
     if(!::MakeFont(font, facename, height, weight, dc))
        { /* failed */
         delete font;
         return FALSE;
        } /* failed */
     SetBaseFont(font);
     return TRUE;
    }

/****************************************************************************
*                               CFontObj::Free
* Result: void
*       
* Effect: 
*       Frees up all resources associated with 'this'.
****************************************************************************/

void CFontObj::Free()
    {
     DeleteBaseFont();
     DeleteNewFont();
     DeleteApplyFont();
    }

/****************************************************************************
*                            CFontObj::GetDescription
* Result: CString * 
*       The description string for the user
****************************************************************************/

CString * CFontObj::GetDescription() 
    {
     if(DescriptionString.GetLength() == 0)
        DescriptionString = getString(Description);
     return &DescriptionString;
    }

/****************************************************************************
*                             CFontObj::getString
* Inputs:
*       LPCTSTR str: String, MAKEINTRESOURCE of string, or NULL
* Result: CString
*       Created string object
* Effect: 
*       Creates and initializes a CString.  If HIWORD(str) == 0, it is treated
*       as a MAKEINTRESOURCE of the string ID
* Notes:
*       We have to defer this to later instead of during creation because
*       the instance handle required by LoadString is not initialized when
*       the static constructors are executed.
****************************************************************************/

CString CFontObj::getString(LPCTSTR str)
    {
     CString s;

     if(str == NULL)
        s = _T("");
     else
     if(HIWORD(str) == 0)
        s.LoadString(LOWORD(str));
     else
        s = str;

     return s;
    }

/****************************************************************************
*                            CFontObj::GetSample
* Result: CString * const
*       The sample string for the CFontDialog or other uses
****************************************************************************/

CString * CFontObj::GetSample() 
    {
     if(SampleString.GetLength() == 0)
        SampleString = getString(Sample);
     return &SampleString;
    }
