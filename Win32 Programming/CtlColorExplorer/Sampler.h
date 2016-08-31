// Sampler.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CSampler dialog

class CSampler : public CPropertyPage
{
        DECLARE_DYNCREATE(CSampler)

// Construction
public:
        CSampler();
        ~CSampler();

// Dialog Data
        //{{AFX_DATA(CSampler)
        enum { IDD = IDD_SAMPLE };
        CEdit   c_MultiLine;
        CStatic c_DisabledCaption;
        CStatic c_ReadOnlyCaption;
        CStatic c_NormalCaption;
        CButton c_Radio3;
        CButton c_Radio2;
        CButton c_Radio1;
        CSliderCtrl     c_Slider1;
        CScrollBar      c_Scrollbar1;
        CProgressCtrl   c_Progress1;
        CListBox        c_List1;
        CButton c_Button;
        CButton c_Check;
        CEdit   c_Edit_ReadOnly;
        CEdit   c_Edit_Disabled;
        CButton c_Group;
        CEdit   c_Edit_Normal;
        CComboBox       c_Combo1;
        CComboBox       c_Combo2;
        CComboBox       c_Combo3;
        //}}AFX_DATA


// Overrides
        // ClassWizard generate virtual function overrides
        //{{AFX_VIRTUAL(CSampler)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        CFont font;

        // Generated message map functions
        //{{AFX_MSG(CSampler)
        afx_msg void OnButton();
        virtual BOOL OnInitDialog();
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
        afx_msg void OnCheck1();
        afx_msg void OnDestroy();
        afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
        afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
        //}}AFX_MSG
        void enableControls(BOOL enable);
        int moveScrollBar(UINT nSBCode, UINT nPos, int bottom, int top, int pos, int delta) ;
        DECLARE_MESSAGE_MAP()

};
