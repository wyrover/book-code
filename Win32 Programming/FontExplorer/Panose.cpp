#include "stdafx.h"
#include "panose.h"

/****************************************************************************
*                                 loadPanose
* Inputs:
*       HTreeCtrl tc: Tree control
*       HTREEITEM ti: Tree item under which to install Panose information
*       PANOSE * pn: PANOSE object
* Result: void
*       
* Effect: 
*       Loads the PANOSE information
****************************************************************************/

void loadPanose(CTreeCtrl * tc, HTREEITEM ti, PANOSE * pn)
    {
     CString s;

     s.Format(_T("bFamilyType 0x%02x"), pn->bFamilyType);
     tc->InsertItem(s, ti);

     s.Format(_T("bSerifStyle 0x%02x"), pn->bSerifStyle);
     tc->InsertItem(s, ti);

     s.Format(_T("bWeight 0x%02x"), pn->bWeight);
     tc->InsertItem(s, ti);

     s.Format(_T("bProportion 0x%02x"), pn->bProportion);
     tc->InsertItem(s, ti);

     s.Format(_T("bContrast 0x%02x"), pn->bContrast);
     tc->InsertItem(s, ti);

     s.Format(_T("bStrokeVariation 0x%02x"), pn->bStrokeVariation);
     tc->InsertItem(s, ti);

     s.Format(_T("bArmStyle 0x%02x"), pn->bArmStyle);
     tc->InsertItem(s, ti);

     s.Format(_T("bLetterform 0x%02x"), pn->bLetterform);
     tc->InsertItem(s, ti);


     s.Format(_T("bMidline 0x%02x"), pn->bMidline);
     tc->InsertItem(s, ti);

     s.Format(_T("bXHeight 0x%02x"), pn->bXHeight);
     tc->InsertItem(s, ti);
     
    }
