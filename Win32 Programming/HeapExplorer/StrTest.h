// StrTest.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStringTest dialog

class CStringTest : public CDialog
{
// Construction
public:
        CStringTest(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
        //{{AFX_DATA(CStringTest)
        enum { IDD = IDD_CMYSTRING };
        CLegend c_Legend;
        CButton c_Dealloc;
        CButton c_Compact;
        CMemoryMap      c_Layout;
        CEdit   c_Input;
        CStringDisplay  c_Strings;
        CComboBox       c_Info;
        CButton c_Delete;
        CButton c_Add;
        //}}AFX_DATA


// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CStringTest)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        void heap_to_display();
        void enableControls();
        void syncInfoToStrings();
        void syncStringsToInfo();
        BOOL inputExists();

        // Generated message map functions
        //{{AFX_MSG(CStringTest)
        afx_msg void OnAdd();
        afx_msg void OnDelete();
        virtual BOOL OnInitDialog();
        afx_msg void OnSelendcancelInfo();
        afx_msg void OnSelendokInfo();
        afx_msg void OnSelendokStrings();
        afx_msg void OnChangeInput();
        afx_msg void OnCompact();
        afx_msg void OnAutodealloc();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};
