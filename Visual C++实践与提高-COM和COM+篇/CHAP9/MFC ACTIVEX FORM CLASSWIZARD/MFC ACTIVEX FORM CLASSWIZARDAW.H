#if !defined(AFX_MFCACTIVEXFORMCLASSWIZARDAW_H__2CA52F29_F9A8_11D1_96ED_F05F03C10000__INCLUDED_)
#define AFX_MFCACTIVEXFORMCLASSWIZARDAW_H__2CA52F29_F9A8_11D1_96ED_F05F03C10000__INCLUDED_

// MFC ActiveX Form ClassWizardaw.h : header file
//

class CDialogChooser;

// All function calls made by mfcapwz.dll to this custom AppWizard (except for
//  GetCustomAppWizClass-- see MFC ActiveX Form ClassWizard.cpp) are through this class.  You may
//  choose to override more of the CCustomAppWiz virtual functions here to
//  further specialize the behavior of this custom AppWizard.
class CMFCActiveXFormClassWizardAppWiz : public CCustomAppWiz
{
public:
    virtual CAppWizStepDlg* Next(CAppWizStepDlg* pDlg);

    virtual void InitCustomAppWiz();
    virtual void ExitCustomAppWiz();
    virtual void CustomizeProject(IBuildProject* pProject);
};

// This declares the one instance of the CMFCActiveXFormClassWizardAppWiz class.  You can access
//  m_Dictionary and any other public members of this class through the
//  global MFCActiveXFormClassWizardaw.  (Its definition is in MFC ActiveX Form ClassWizardaw.cpp.)
extern CMFCActiveXFormClassWizardAppWiz MFCActiveXFormClassWizardaw;

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCACTIVEXFORMCLASSWIZARDAW_H__2CA52F29_F9A8_11D1_96ED_F05F03C10000__INCLUDED_)
