// StatTest.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticTest dialog

class CStaticTest : public CPropertyPage
{
        DECLARE_DYNCREATE(CStaticTest)

// Construction
public:
        CStaticTest();
        ~CStaticTest();

// Dialog Data
        //{{AFX_DATA(CStaticTest)
        enum { IDD = IDD_STATIC };
        CButton c_SSNoPrefix;
        CButton c_WSBorder;
        CButton c_SSBitmap;
        CButton c_SSWhiteRect;
        CButton c_SSWhiteFrame;
        CButton c_SSUserItem;
        CButton c_SSSimple;
        CButton c_SSRight;
        CButton c_SSOwnerDraw;
        CButton c_SSBlackFrame;
        CButton c_SSBlackRect;
        CButton c_SSCenter;
        CButton c_SSCenterImage;
        CButton c_SSEnhMetaFile;
        CButton c_SSEtchedFrame;
        CButton c_SSEtchedHorz;
        CButton c_SSEtchedVert;
        CButton c_SSGrayFrame;
        CButton c_SSGrayRect;
        CButton c_SSIcon;
        CButton c_SSLeft;
        CButton c_SSLeftNoWordWrap;
        CButton c_Enable;
        CEdit   c_Edit;
        CMyStatic       c_Static;
        CButton c_SSSunken;
        CButton c_SSRightJust;
        CButton c_SSRealSize;
        CButton c_SSNotify;
        CButton c_SetText;
        CButton c_NoAnimation;
        CMessageLog     c_Messages;
        //}}AFX_DATA


// Overrides
        // ClassWizard generate virtual function overrides
        //{{AFX_VIRTUAL(CStaticTest)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        CFont * orgfont;
        CRect orgsize;
        CBitmap bm;

        void recreateWindow(DWORD style);
        void addMessage(CString s);
        void addStyle(CString & s, DWORD styles, DWORD style, int styleid);
        CString styleToString(DWORD style);
        void logSetWindowLong(DWORD style);
        // Generated message map functions
        //{{AFX_MSG(CStaticTest)
        afx_msg void OnStaticEnable();
        afx_msg void OnStaticDisable();
        afx_msg void OnStaticClicked();
        afx_msg void OnStaticDblClicked();
        afx_msg void OnTimer(UINT nIDEvent);
        afx_msg void OnDlgcode();
        afx_msg void OnSettext();
        afx_msg void OnSSBitmap();
        afx_msg void OnSSBlackframe();
        afx_msg void OnSSBlackrect();
        afx_msg void OnSSCenter();
        afx_msg void OnSSCenterimage();
        afx_msg void OnSSEnhmetafile();
        afx_msg void OnSSEtchedframe();
        afx_msg void OnSSEtchedhorz();
        afx_msg void OnSSEtchedvert();
        afx_msg void OnSSGrayframe();
        afx_msg void OnSSGrayrect();
        afx_msg void OnSSIcon();
        afx_msg void OnSSLeft();
        afx_msg void OnSSLeftnowordwrap();
        afx_msg void OnSSNotify();
        afx_msg void OnSSOwnerdraw();
        afx_msg void OnSSRealsizeimage();
        afx_msg void OnSSRight();
        afx_msg void OnSSRightjust();
        afx_msg void OnSSSimple();
        afx_msg void OnSSSunken();
        afx_msg void OnSSUseritem();
        afx_msg void OnSSWhiteframe();
        afx_msg void OnSSWhiterect();
        afx_msg void OnEnabled();
        virtual BOOL OnInitDialog();
        afx_msg void OnWsBorder();
        afx_msg void OnSsNoprefix();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

};
