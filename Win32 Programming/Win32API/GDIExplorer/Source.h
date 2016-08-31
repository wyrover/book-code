// Source.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSource window

class CSource : public CStatic
{
// Construction
public:
        CSource();
        BOOL setBitmap(int bmid);
        BOOL setBitmap(CBitmap * bm);
        void clearBitmap(int mode = 1);
        CBitmap * getBitmap();
        void createBitmap();
        int setBorder(int border);
// Attributes
public:

// Operations
public:
        void getObjectRect(CRect * r);
        void getObjectWindowRect(CRect * r);
 
// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CSource)
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CSource();

        // Generated message map functions
protected:
        CBitmap bitmap;
        BITMAPINFO bmi;
        int factor;  // border ratio factor

        //{{AFX_MSG(CSource)
        afx_msg void OnDestroy();
        afx_msg void OnPaint();
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
