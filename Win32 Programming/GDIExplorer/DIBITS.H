// DIBits.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDIBits dialog

class CDIBits : public CPropertyPage
{
        DECLARE_DYNCREATE(CDIBits)

// Construction
public:
        CDIBits();
        ~CDIBits();

// Dialog Data
        //{{AFX_DATA(CDIBits)
        enum { IDD = IDD_DIBITS };
        CIntEdit        c_biYPelsPerMeter;
        CIntEdit        c_biXPelsPerMeter;
        CIntEdit        c_biWidth;
        CIntEdit        c_biSizeImage;
        CIntEdit        c_biSize;
        CIntEdit        c_biPlanes;
        CIntEdit        c_biHeight;
        CEdit   c_biCompression;
        CIntEdit        c_biBitCount;
        CIntEdit        c_biClrImportant;
        CIntEdit        c_biClrUsed;
        CSource c_Source;
        CBitmapSelect   c_BitmapSelect;
        //}}AFX_DATA


// Overrides
        // ClassWizard generate virtual function overrides
        //{{AFX_VIRTUAL(CDIBits)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        void updateSource();
        CBitmap bitmap;
        // Generated message map functions
        //{{AFX_MSG(CDIBits)
        afx_msg void OnSelendokBitmap();
        virtual BOOL OnInitDialog();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

};
