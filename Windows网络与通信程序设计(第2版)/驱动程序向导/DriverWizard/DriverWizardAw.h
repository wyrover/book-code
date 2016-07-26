#if !defined(AFX_DRIVERWIZARDAW_H__1029BBE7_10D9_40F5_827D_54437FD9E282__INCLUDED_)
#define AFX_DRIVERWIZARDAW_H__1029BBE7_10D9_40F5_827D_54437FD9E282__INCLUDED_

// DriverWizardaw.h : header file
//

class CDialogChooser;

// All function calls made by mfcapwz.dll to this custom AppWizard (except for
//  GetCustomAppWizClass-- see DriverWizard.cpp) are through this class.  You may
//  choose to override more of the CCustomAppWiz virtual functions here to
//  further specialize the behavior of this custom AppWizard.
class CDriverWizardAppWiz : public CCustomAppWiz
{
public:
	virtual CAppWizStepDlg* Next(CAppWizStepDlg* pDlg);
		
	virtual void InitCustomAppWiz();
	virtual void ExitCustomAppWiz();
	virtual void CustomizeProject(IBuildProject* pProject);
};

// This declares the one instance of the CDriverWizardAppWiz class.  You can access
//  m_Dictionary and any other public members of this class through the
//  global DriverWizardaw.  (Its definition is in DriverWizardaw.cpp.)
extern CDriverWizardAppWiz DriverWizardaw;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRIVERWIZARDAW_H__1029BBE7_10D9_40F5_827D_54437FD9E282__INCLUDED_)
