// Bitmaps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBitmaps window

class CBitmaps : public CComboBox
{
// Construction
public:
        CBitmaps();
        void load();
        int Select(HMENU menu);
        HBITMAP GetCurItem();
// Attributes
public:

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CBitmaps)
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CBitmaps();

        // Generated message map functions
protected:
        //{{AFX_MSG(CBitmaps)
        afx_msg void OnDestroy();
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
