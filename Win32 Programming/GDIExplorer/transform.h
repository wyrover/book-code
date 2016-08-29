// transfor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTransform dialog

class CTransform : public CDialog
{
// Construction
public:
        CTransform(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
        //{{AFX_DATA(CTransform)
        enum { IDD = IDD_TRANSFORM };
        CMatrixDisplay  c_Stack6;
        CMatrixDisplay  c_Stack5;
        CMatrixDisplay  c_Stack4;
        CMatrixDisplay  c_Stack3;
        CMatrixDisplay  c_Stack2;
        CMatrixDisplay  c_Stack1;
        CMatrixDisplay  c_Stack0;
        CMatrixDisplay  c_CTM;
        CSpinButtonCtrl c_M22s;
        CComboBox       c_M22c;
        CEdit   c_M22;
        CSpinButtonCtrl c_M21s;
        CComboBox       c_M21c;
        CEdit   c_M21;
        CEdit   c_DX;
        CEdit   c_DY;
        CSpinButtonCtrl c_M12s;
        CComboBox       c_M12c;
        CEdit   c_M12;
        CEdit   c_M11;
        CComboBox       c_M11c;
        CSpinButtonCtrl c_M11s;
        CSpinButtonCtrl c_DXs;
        CSpinButtonCtrl c_DYs;
        //}}AFX_DATA

        CXform matrix;

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CTransform)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:

        // Generated message map functions
        //{{AFX_MSG(CTransform)
        afx_msg void OnChangeDx();
        afx_msg void OnChangeDy();
        afx_msg void OnIdentity();
        afx_msg void OnChangeM11();
        afx_msg void OnChangeM12();
        afx_msg void OnChangeM21();
        afx_msg void OnChangeM22();
        afx_msg void OnRotate();
        afx_msg void OnSelchangeM11c();
        afx_msg void OnSelchangeM12c();
        afx_msg void OnSelchangeM21c();
        afx_msg void OnSelchangeM22c();
        afx_msg void OnCTMx();
        afx_msg void OnXctm();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};
